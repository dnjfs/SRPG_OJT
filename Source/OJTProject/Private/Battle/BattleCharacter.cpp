// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"

ABattleCharacter::ABattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABattleAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->MaxAcceleration = 10000;
	GetCharacterMovement()->MaxWalkSpeed= 1000;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UMG/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
}

void ABattleCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CurrentHP = MaxHP; //현재 HP는 풀피로

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	auto GameInst = Cast<UMapGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); //생성자에서 GetWorld() 호출 시 CDO를 만들 때 월드가 없어 nullptr이 반환됨
	if (GameInst == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetGameInstance() return NULLPTR"));
		return;
	}

	FCharacterTable* CharacterRow = GameInst->GetCharcaterData(CharacterType);
	if (CharacterRow != nullptr)
	{
		GetMesh()->SetSkeletalMesh(CharacterRow->SKChar);
		GetMesh()->SetAnimInstanceClass(CharacterRow->AIChar);
		PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		//PlayerAnim->SetAttackMontage(CharacterRow->AMAttack);
		//PlayerAnim->SetSkillMontage(CharacterRow->AMSkill);
		//PlayerAnim->SetHitMontage(CharacterRow->AMHit);
		//UE_LOG(LogTemp, Warning, TEXT("ABattleCharacter::PostInitializeComponents(): Complete Load Asset %s"), *CharacterRow->AMChar->GetName());

		Cast<ABattleAIController>(GetController())->SetEndAttackDelegate();
		PlayerAnim->OnAttackHit.AddUObject(this, &ABattleCharacter::AttackCharacter);
		PlayerAnim->OnSkillHit.AddUObject(this, &ABattleCharacter::SkillCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABattleCharacter::ABattleCharacter() : CharacterRow is NULL"));
	}
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	HPBar = Cast<UProgressBar>(HPBarWidget->GetUserWidgetObject()->GetWidgetFromName(TEXT("PB_HPBAR")));
	HPBar->SetPercent((float)CurrentHP / MaxHP);
}

void ABattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("ABattleCharacter::SetupPlayerInputComponent"));
}

float ABattleCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangeHP(FinalDamage);

	return FinalDamage;
}

void ABattleCharacter::SetCharacterType(ECharacterType inType)
{
	CharacterType = inType;
}

void ABattleCharacter::SetTileLocationID(int onLoc)
{
	TileLocID = onLoc;
}
int32 ABattleCharacter::GetTileLocationID()
{
	return TileLocID;
}

void ABattleCharacter::SetPlayerCharacter(bool inIsPlayer)
{
	bIsPlayer = inIsPlayer;
}
bool ABattleCharacter::GetIsPlayer()
{
	return bIsPlayer;
}

void ABattleCharacter::SetTargetCharacter(ABattleCharacter* inTarget)
{
	TargetCharacter = inTarget;
}

void ABattleCharacter::PlayAttackAnimation()
{
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetCharacter->GetActorLocation());
	SetActorRotation(rotator); //타겟 방향으로 회전

	PlayerAnim->PlayAttackMontage();
}

void ABattleCharacter::PlaySkillAnimation()
{
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetCharacter->GetActorLocation());
	SetActorRotation(rotator); //타겟 방향으로 회전

	PlayerAnim->PlaySkillMontage();
}

void ABattleCharacter::AttackCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Attack %s"), *GetName(), *TargetCharacter->GetName());
	
	FDamageEvent DamageEvent;
	TargetCharacter->TakeDamage(Power, DamageEvent, GetController(), this); //타겟 받아서 TakeDamage() 호출

	//TargetCharacter = nullptr; //공격완료 후 대상 초기화
}

void ABattleCharacter::SkillCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Skill %s"), *GetName(), *TargetCharacter->GetName());

	FDamageEvent DamageEvent;
	TargetCharacter->TakeDamage(Power * Coefficient, DamageEvent, GetController(), this);
}

void ABattleCharacter::ChangeHP(float Damage)
{
	if(CurrentHP > 0)
	{
		CurrentHP -= Damage;
		HPBar->SetPercent((float)CurrentHP / MaxHP);
		
		//받은 데미지 UI로 출력, 1초 뒤에 사라지도록

		if(CurrentHP <= 0) //HP < KINDA_SMALL_NUMBER
		{
			CurrentHP = 0;
			DeadCharacter();

			return;
		}
		PlayerAnim->PlayHitMontage();
		UE_LOG(LogTemp, Warning, TEXT("%s's HP is %d"), *GetName(), CurrentHP);
	}
}

void ABattleCharacter::DeadCharacter()
{
	PlayerAnim->SetIsDead();

	SetLifeSpan(2.0f);
	OnNotifyDeadDelegate.Broadcast(TileLocID); //타일에서 캐릭터 지우기
	//아래 함수들을 2초 후에 부를 순 없을까?
	//GetGameInstance()->GetTimerManager().SetTimer();
	//SetActorHiddenInGame(true);
	//SetActorEnableCollision(false);
	//SetActorTickEnabled(false);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

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
		PlayerAnim->OnAttackHit.AddUObject(this, &ABattleCharacter::AttackCharacter); //애님 노티파이에서 호출
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
	PrintHitDamage(FinalDamage);

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

void ABattleCharacter::SetAttackState()
{
	AttackType = EAttackType::ATTACK;
	//PlayAnimationMontage();
}
void ABattleCharacter::SetSkillState()
{
	AttackType = EAttackType::SKILL;
	//PlayAnimationMontage();
}
bool ABattleCharacter::IsSkillState()
{
	if(AttackType == EAttackType::SKILL)
	{
		return true;
	}
	return false;
}

void ABattleCharacter::PlayAnimationMontage()
{
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetCharacter->GetActorLocation());
	SetActorRotation(rotator); //타겟 방향으로 회전

	if(AttackType == EAttackType::ATTACK)
	{
		PlayerAnim->PlayAttackMontage();
	}
	else if (AttackType == EAttackType::SKILL)
	{
		PlayerAnim->PlaySkillMontage();
	}
}

void ABattleCharacter::AttackCharacter()
{
	FDamageEvent DamageEvent;
	if (AttackType == EAttackType::ATTACK)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Attack %s"), *GetName(), *TargetCharacter->GetName());
		TargetCharacter->TakeDamage(Power, DamageEvent, GetController(), this); //타겟 받아서 TakeDamage() 호출
	}
	else if (AttackType == EAttackType::SKILL)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Skill %s"), *GetName(), *TargetCharacter->GetName());
		TargetCharacter->TakeDamage(Power * Coefficient, DamageEvent, GetController(), this);
	}

	//TargetCharacter = nullptr; //공격완료 후 대상 초기화
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
void ABattleCharacter::PrintHitDamage(int Damage)
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprints/BP_HitText.BP_HitText'")));
	if (!SpawnActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	UClass* SpawnClass = SpawnActor->StaticClass();
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* TextActor = GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, GetActorTransform().GetLocation() + FVector(0, 0, 100), FRotator(0, 90, 0));
	if (Cast<UTextRenderComponent>(TextActor->GetRootComponent()) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UTextRenderComponent is NULL!!!!!!!!"));
		return;
	}
	Cast<UTextRenderComponent>(TextActor->GetRootComponent())->SetText(FString::FromInt(Damage));
}

void ABattleCharacter::DeadCharacter()
{
	PlayerAnim->SetIsDead();

	SetLifeSpan(2.0f);
	OnNotifyDeadDelegate.Broadcast(TileLocID); //타일에서 캐릭터 지우기
}
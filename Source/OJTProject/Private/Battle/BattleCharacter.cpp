// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"
#include "Battle/PlayerAnimInstance.h"

ABattleCharacter::ABattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABattleAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CharacterType = ECharacterType::PLAYER1;
	HP = 100;
	Power = 40;
}

void ABattleCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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
		Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetAttackMontage(CharacterRow->AMChar);
		//UE_LOG(LogTemp, Warning, TEXT("ABattleCharacter::PostInitializeComponents(): Complete Load Asset %s"), *CharacterRow->AMChar->GetName());

		Cast<ABattleAIController>(GetController())->SetEndAttackDelegate();
		Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->OnAttackHit.AddUObject(this, &ABattleCharacter::AttackCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABattleCharacter::ABattleCharacter() : CharacterRow is NULL"));
	}
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void ABattleCharacter::AttackCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Attack %s"), *GetName(), *TargetCharacter->GetName());
	//공격 방향으로 회전
	//타겟 받아서 TakeDamage() 호출

	FDamageEvent DamageEvent;
	TargetCharacter->TakeDamage(Power, DamageEvent, GetController(), this);

	//TargetCharacter = nullptr; //공격완료 후 대상 초기화
}

void ABattleCharacter::ChangeHP(float Damage)
{
	HP -= Damage;
	if(HP < 0)
	{
		HP = 0;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s's HP is %d"), *GetName(), HP);
}
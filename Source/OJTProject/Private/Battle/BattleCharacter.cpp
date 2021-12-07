// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"

ABattleCharacter::ABattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABattleAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABattleCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	auto GameInst = Cast<UMapGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); //�����ڿ��� GetWorld() ȣ�� �� CDO�� ���� �� ���尡 ���� nullptr�� ��ȯ��
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
	//���� �������� ȸ��
	PlayerAnim->PlayAttackMontage();
}

void ABattleCharacter::AttackCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Attack %s"), *GetName(), *TargetCharacter->GetName());
	
	FDamageEvent DamageEvent;
	TargetCharacter->TakeDamage(Power, DamageEvent, GetController(), this); //Ÿ�� �޾Ƽ� TakeDamage() ȣ��

	//TargetCharacter = nullptr; //���ݿϷ� �� ��� �ʱ�ȭ
}

void ABattleCharacter::ChangeHP(float Damage)
{
	if(HP > 0)
	{
		HP -= Damage;
		if(HP < 0)
		{
			HP = 0;
			DeadCharacter();

			return;
		}
		PlayerAnim->PlayHitMontage();
		UE_LOG(LogTemp, Warning, TEXT("%s's HP is %d"), *GetName(), HP);
	}
}

void ABattleCharacter::DeadCharacter()
{
	PlayerAnim->SetIsDead();

	SetLifeSpan(2.0f);
	OnNotifyDeadDelegate.Broadcast(TileLocID); //Ÿ�Ͽ��� ĳ���� �����
	//�Ʒ� �Լ����� 2�� �Ŀ� �θ� �� ������?
	//GetGameInstance()->GetTimerManager().SetTimer();
	//SetActorHiddenInGame(true);
	//SetActorEnableCollision(false);
	//SetActorTickEnabled(false);
}
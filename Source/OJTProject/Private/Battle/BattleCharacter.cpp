// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"

// Sets default values
ABattleCharacter::ABattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//SkeletalMesh 가져오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_MANNEQUIN.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("SK Success"));
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SK Failed"));
	}

	//AnimInstance 가져오기
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> MANNEQUIN_ANIM(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'")); //블루프린트 가져올 땐 _C 붙이기
	if (MANNEQUIN_ANIM.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("ANIM Success"));
		GetMesh()->SetAnimInstanceClass(MANNEQUIN_ANIM.Class);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ANIM Failed"));
	}

	AIControllerClass = ABattleAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HP = 100;
	Power = 40;
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetActorLocation(GetActorLocation()+ FVector(10*DeltaTime, 0, 0)); //단순히 움직임, 캐릭터 무브먼트를 이용하는 방법은?
}

// Called to bind functionality to input
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("ABattleCharacter::SetupPlayerInputComponent"));
}

void ABattleCharacter::SetTileLocation(int onLoc)
{
	TileLoc = onLoc;
}
int32 ABattleCharacter::GetTileLocation()
{
	return TileLoc;
}

void ABattleCharacter::SetPlayerCharacter(bool inIsPlayer)
{
	bIsPlayer = inIsPlayer;
}
bool ABattleCharacter::GetIsPlayer()
{
	return bIsPlayer;
}

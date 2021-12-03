// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"

ABattleCharacter::ABattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABattleAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CharacterType = ECharacterType::PLAYER1;
	HP = 100;
	Power = 40;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	auto GameInst = Cast<UMapGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
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

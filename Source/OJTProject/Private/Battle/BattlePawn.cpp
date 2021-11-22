// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattlePawn.h"

// Sets default values
ABattlePawn::ABattlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("ABattlePawn::Constructor()"));
}

void ABattlePawn::PostInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePawn::PostInitializeComponents"));
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("2ABattlePawn::PostInitializeComponents"));
}

// Called when the game starts or when spawned
void ABattlePawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ABattlePawn::BeginPlay"));

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		UE_LOG(LogTemp, Warning, TEXT("ABattlePawn::BattleMode PC: %s"), *It->Get()->GetName());
}

// Called every frame
void ABattlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABattlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePawn::SetupPlayerInputController: %s"), *PlayerInputComponent->GetName());
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("2ABattlePawn::SetupPlayerInputController: %s"), *PlayerInputComponent->GetName());
}

void ABattlePawn::PossessedBy(AController* NewController)
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePawn::PossessedBy: %s"), *NewController->GetName());
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Warning, TEXT("2ABattlePawn::PossessedBy: %s"), *NewController->GetName());
}
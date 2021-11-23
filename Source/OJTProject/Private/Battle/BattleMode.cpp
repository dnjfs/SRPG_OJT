// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleMode.h"
#include "Battle/BattleCharacter.h"
//#include "Battle/BattlePawn.h"
#include "Battle/BattlePlayerController.h"
#include "Battle/BattleState.h"
#include "Battle/BattlePlayerState.h"

ABattleMode::ABattleMode()
{
	DefaultPawnClass = NULL;
	//DefaultPawnClass = ABattlePawn::StaticClass();
	//DefaultPawnClass = ABattleCharacter::StaticClass();
	PlayerControllerClass = ABattlePlayerController::StaticClass();
	GameStateClass = ABattleState::StaticClass();
	PlayerStateClass = ABattlePlayerState::StaticClass();

	UE_LOG(LogTemp, Warning, TEXT("ABattleMode::Constructor()"));
}

void ABattleMode::PostInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattleMode::PostInitializeComponents"));
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("2ABattleMode::PostInitializeComponents"));
}

void ABattleMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("ABattleMode::PostLogin / NewPlayer: %s"), *NewPlayer->GetName());
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("2ABattleMode::PostLogin / NewPlayer: %s"), *NewPlayer->GetName());
}

void ABattleMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABattleMode::BeginPlay / PC: %s"), *PlayerControllerClass->GetName());
}
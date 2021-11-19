// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleMode.h"
#include "Battle/BattlePlayerController.h"
#include "Battle/BattleState.h"
#include "Battle/BattlePlayerState.h"

ABattleMode::ABattleMode()
{
	DefaultPawnClass = NULL;
	PlayerControllerClass = ABattlePlayerController::StaticClass();
	GameStateClass = ABattleState::StaticClass();
	PlayerStateClass = ABattlePlayerState::StaticClass();
}

void ABattleMode::BeginPlay()
{
	Super::BeginPlay();

}
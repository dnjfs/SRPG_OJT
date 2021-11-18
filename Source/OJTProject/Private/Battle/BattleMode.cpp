// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleMode.h"
#include "Battle/BattlePlayerController.h"
#include "Battle/BattleState.h"

ABattleMode::ABattleMode()
{
	PlayerControllerClass = ABattlePlayerController::StaticClass();
	GameStateClass = ABattleState::StaticClass();
}

void ABattleMode::BeginPlay()
{
	Super::BeginPlay();

}
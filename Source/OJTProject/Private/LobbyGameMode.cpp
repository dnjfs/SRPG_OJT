// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyUIController.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyUIController::StaticClass();
}
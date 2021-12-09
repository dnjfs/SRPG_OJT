// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyUIController.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyUIController::StaticClass();

	//로비 입장 시 배경음악, 배경이미지 출력
}
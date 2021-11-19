// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattlePlayerController.h"

void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ABattlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("ClickTile", IE_Pressed, this, &ABattlePlayerController::OnClickTile);
}

void ABattlePlayerController::OnClickTile()
{
	FHitResult HitResult;
	this->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	UE_LOG(LogTemp, Warning, TEXT("%f %f"), HitResult.Location.X, HitResult.Location.Y);
}
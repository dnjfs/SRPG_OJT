// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattlePlayerController.h"

ABattlePlayerController::ABattlePlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::Constructor()"));
}

void ABattlePlayerController::PostInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::PostInitializeComponents"));
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("2ABattlePlayerController::PostInitializeComponents"));
}

void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(GameAndUIInputMode); //입력모드 설정해야 로비에서 진입해도 제대로 입력됨

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::BeginPlay"));
}

void ABattlePlayerController::SetupInputComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::SetupInputComponent"));
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("2ABattlePlayerController::SetupInputComponent"));

	InputComponent->BindAction(TEXT("ClickTile"), EInputEvent::IE_Pressed, this, &ABattlePlayerController::OnClickTile);
}

void ABattlePlayerController::OnClickTile()
{
	FHitResult HitResult;
	this->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	UE_LOG(LogTemp, Warning, TEXT("%f %f"), HitResult.Location.X, HitResult.Location.Y);
}

void ABattlePlayerController::OnPossess(APawn* aPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::OnPossess: %s"), *aPawn->GetName());
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Warning, TEXT("2ABattlePlayerController::OnPossess: %s"), *aPawn->GetName());
}
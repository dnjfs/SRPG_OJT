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
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::BeginPlay"));

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	GameAndUIInputMode.SetHideCursorDuringCapture(false); //클릭 중 마우스 고정 풀기
	SetInputMode(GameAndUIInputMode); //입력모드 설정해야 로비에서 진입해도 제대로 입력됨
}

void ABattlePlayerController::SetupInputComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::SetupInputComponent"));
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("2ABattlePlayerController::SetupInputComponent"));

	//InputComponent->BindAction(TEXT("ClickTile"), EInputEvent::IE_Pressed, this, &ABattlePlayerController::OnClickTile);
	//InputComponent->BindAction(TEXT("DragTile"), EInputEvent::IE_Repeat, this, &ABattlePlayerController::OnDragTile);
	//InputComponent->BindAction(TEXT("EndTile"), EInputEvent::IE_Released, this, &ABattlePlayerController::OnEndTile);
}

//void ABattlePlayerController::OnClickTile()
//{
//	//UKismetSystemLibrary::LineTraceSingleForObjects();
//	FHitResult HitResult;
//	this->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
//	UE_LOG(LogTemp, Warning, TEXT("Click %f %f"), HitResult.Location.X, HitResult.Location.Y);
//}
//void ABattlePlayerController::OnDragTile()
//{
//	FHitResult HitResult;
//	this->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
//	UE_LOG(LogTemp, Warning, TEXT("Drag %f %f"), HitResult.Location.X, HitResult.Location.Y);
//}
//void ABattlePlayerController::OnEndTile()
//{
//	FHitResult HitResult;
//	this->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
//	UE_LOG(LogTemp, Warning, TEXT("End %f %f"), HitResult.Location.X, HitResult.Location.Y);
//}

void ABattlePlayerController::OnPossess(APawn* aPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::OnPossess: %s"), *aPawn->GetName());
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Warning, TEXT("2ABattlePlayerController::OnPossess: %s"), *aPawn->GetName());
}
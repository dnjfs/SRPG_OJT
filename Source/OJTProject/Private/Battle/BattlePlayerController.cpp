// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattlePlayerController.h"
#include "UI/GamePlayWidget.h"
#include "UI/ResultWidget.h"

ABattlePlayerController::ABattlePlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattlePlayerController::Constructor()"));

	static ConstructorHelpers::FClassFinder<UGamePlayWidget> UI_GAMEPLAY(TEXT("WidgetBlueprint'/Game/UMG/UI_GamePlay.UI_GamePlay_C'"));
	if (UI_GAMEPLAY.Succeeded())
	{
		GamePlayWidgetClass = UI_GAMEPLAY.Class;
	}

	static ConstructorHelpers::FClassFinder<UResultWidget> UI_RESULT(TEXT("WidgetBlueprint'/Game/UMG/UI_Result.UI_Result_C'"));
	if(UI_RESULT.Succeeded())
	{
		ResultWidgetClass = UI_RESULT.Class;
	}
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

	GameAndUIInputMode.SetHideCursorDuringCapture(false); //Ŭ�� �� ���콺 ���� Ǯ��
	SetInputMode(GameAndUIInputMode); //�Է¸�� �����ؾ� �κ񿡼� �����ص� ����� �Էµ�

	//���� ����
	GamePlayWidgetInstance = CreateWidget<UGamePlayWidget>(this, GamePlayWidgetClass);
	GamePlayWidgetInstance->AddToViewport();

	GamePlayWidgetInstance->OnGamePauseDelegate.AddLambda([this]() {
		if(IsPaused())
		{
			SetPause(false);
			SetInputMode(GameAndUIInputMode);
		}
		else
		{
			SetPause(true);
			SetInputMode(UIInputMode); //�Ͻ����� ���ȿ� ���ӿ� �Է� �ȵǵ��� UI�Է¸��� ��ȯ
		}
	});
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

void ABattlePlayerController::ShowResultUI(bool bIsWin, int inTurnCount)
{
	ResultWidgetInstance = CreateWidget<UResultWidget>(this, ResultWidgetClass);
	ResultWidgetInstance->AddToViewport();
	ResultWidgetInstance->SetResultMessage(bIsWin, inTurnCount);
}
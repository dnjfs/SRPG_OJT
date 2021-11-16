// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUIController.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.h"

ALobbyUIController::ALobbyUIController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_LOBBY(TEXT("/Game/UMG/UI_Lobby.UI_Lobby_C"));
	if(UI_LOBBY.Succeeded())
	{
		UIWidgetClass = UI_LOBBY.Class;
	}
}

void ALobbyUIController::BeginPlay()
{
	Super::BeginPlay();

	if (UIWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UIWidgetClass is NULL!"));
		return;
	}

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	if (UIWidgetInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UIWidgetInstance is NULL!"));
		return;
	}

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
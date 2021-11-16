// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUIController.h"
#include "Blueprint/UserWidget.h"

void ATitleUIController::BeginPlay()
{
	Super::BeginPlay();

	//check(UIWidgetClass != nullptr);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	//check(UIWidgetInstance != nullptr);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
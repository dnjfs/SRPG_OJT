// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamePlayWidget.h"
#include "Battle/BattleState.h"
#include "Components/Button.h"

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PauseButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPause")));
	if (PauseButton != nullptr)
	{
		PauseButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnGamePause);
	}

	//BattleState = Cast<ABattleState>(UGameplayStatics::GetGameState(this));
}

void UGamePlayWidget::OnGamePause()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause Clicked!"));
	OnGamePauseDelegate.Broadcast();
}
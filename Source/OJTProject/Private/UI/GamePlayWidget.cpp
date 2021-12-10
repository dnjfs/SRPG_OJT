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

	BattleState = Cast<ABattleState>(UGameplayStatics::GetGameState(this));
	SkillButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSkill")));
	if (SkillButton != nullptr)
	{
		SkillButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnActiveSkill);
	}

	BattleState->OnTurnCountDelegate.AddDynamic(this, &UGamePlayWidget::UpdateTurnCount);
}

void UGamePlayWidget::OnGamePause()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause Clicked!"));
	OnGamePauseDelegate.Broadcast();
}

void UGamePlayWidget::OnActiveSkill()
{
	BattleState->ActiveSkill();
}

void UGamePlayWidget::UpdateTurnCount(int TurnCount)
{
	UE_LOG(LogTemp, Warning, TEXT("UGamePlayWidget::UpdateTurnCount(%d)"), TurnCount);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamePlayWidget.h"
#include "Battle/BattleState.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Error, TEXT("UGamePlayWidget::NativeConstruct()"));

	TurnText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTurn")));

	PauseButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPause")));
	if (PauseButton != nullptr)
	{
		PauseButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnGamePause);
	}

	SkillButton = Cast<UButton>(GetWidgetFromName(TEXT("btnSkill")));
	if (SkillButton != nullptr)
	{
		SkillButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnActiveSkill);
	}

	BattleState = Cast<ABattleState>(UGameplayStatics::GetGameState(this));
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
	TurnText->SetText(FText::FromString(TEXT("Turn: ") + FString::FromInt(TurnCount)));
	UE_LOG(LogTemp, Warning, TEXT("UGamePlayWidget::UpdateTurnCount(%d)"), TurnCount);
}
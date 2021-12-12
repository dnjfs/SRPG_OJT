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

	Player1SpawnButton = Cast<UButton>(GetWidgetFromName(TEXT("btnP1")));
	if(Player1SpawnButton != nullptr)
	{
		Player1SpawnButton->OnClicked.AddDynamic(this, &UGamePlayWidget::Player1Spawn);
	}
	Player2SpawnButton = Cast<UButton>(GetWidgetFromName(TEXT("btnP2")));
	if(Player2SpawnButton != nullptr)
	{
		Player2SpawnButton->OnClicked.AddDynamic(this, &UGamePlayWidget::Player2Spawn);
	}
	EnemySpawnButton = Cast<UButton>(GetWidgetFromName(TEXT("btnE")));
	if(EnemySpawnButton != nullptr)
	{
		EnemySpawnButton->OnClicked.AddDynamic(this, &UGamePlayWidget::EnemySpawn);
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
	if(!BattleState->GetIsRunBehavior())
	{
		if(BattleState->ActiveSkill())
		{
			SkillButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			SkillButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

void UGamePlayWidget::UpdateTurnCount(int TurnCount)
{
	SkillButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	TurnText->SetText(FText::FromString(TEXT("Turn: ") + FString::FromInt(TurnCount)));
	UE_LOG(LogTemp, Warning, TEXT("UGamePlayWidget::UpdateTurnCount(%d)"), TurnCount);
}

void UGamePlayWidget::Player1Spawn()
{
	ClearSpawnButton();
	Player1SpawnButton->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
	BattleState->ReadyCharacterSpawn(ECharacterType::PLAYER1);
}
void UGamePlayWidget::Player2Spawn()
{
	ClearSpawnButton();
	Player2SpawnButton->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
	BattleState->ReadyCharacterSpawn(ECharacterType::PLAYER2);
}
void UGamePlayWidget::EnemySpawn()
{
	ClearSpawnButton();
	EnemySpawnButton->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
	BattleState->ReadyCharacterSpawn(ECharacterType::ENEMY);
}
void UGamePlayWidget::ClearSpawnButton()
{
	Player1SpawnButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	Player2SpawnButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	EnemySpawnButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}
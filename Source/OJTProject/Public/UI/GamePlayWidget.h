// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGamePauseDelegate);

/**
 * 
 */
UCLASS()
class OJTPROJECT_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnGamePause();

	UFUNCTION()
	void OnActiveSkill();

	UFUNCTION()
	void UpdateTurnCount(int TurnCount);

	UFUNCTION()
	void Player1Spawn();
	UFUNCTION()
	void Player2Spawn();
	UFUNCTION()
	void EnemySpawn();
	UFUNCTION()
	void ClearSpawnButton();

	FOnGamePauseDelegate OnGamePauseDelegate;

protected:
	UPROPERTY()
	class UTextBlock* TurnText;

	UPROPERTY()
	class UButton* PauseButton;
	
	UPROPERTY()
	class UButton* SkillButton;

	UPROPERTY()
	class UButton* Player1SpawnButton;
	UPROPERTY()
	class UButton* Player2SpawnButton;
	UPROPERTY()
	class UButton* EnemySpawnButton;

private:
	TWeakObjectPtr<class ABattleState> BattleState;
};

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

public:
	FOnGamePauseDelegate OnGamePauseDelegate;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnGamePause();

	UFUNCTION()
	void OnActiveSkill();

protected:
	UPROPERTY()
	class UButton* PauseButton;
	
	UPROPERTY()
	class UButton* SkillButton;

private:
	TWeakObjectPtr<class ABattleState> BattleState;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OJTProject.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnOpenLevel1Clicked();

	UFUNCTION()
	void OnOpenLevel2Clicked();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UButton* Level1;

	UPROPERTY()
	class UButton* Level2;
};

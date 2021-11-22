// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OJTProject.h"
#include "GameFramework/PlayerController.h"
#include "LobbyUIController.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ALobbyUIController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyUIController();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY()
		TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
		class UUserWidget* UIWidgetInstance;
};

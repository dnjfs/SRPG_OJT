// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	TSubclassOf<class UGamePlayWidget> GamePlayWidgetClass;

public:
	ABattlePlayerController();
	virtual void OnPossess(APawn* aPawn) override;
	//void OnClickTile();
	//void OnDragTile();
	//void OnEndTile();

private:
	FInputModeGameAndUI GameAndUIInputMode;
	//FInputModeGameOnly GameInputMode; 사용하려면 GameInputMode.SetConsumeCaptureMouseDown(false) 해주기
	FInputModeUIOnly UIInputMode;

	UPROPERTY()
	class UGamePlayWidget* GamePlayWidgetInstance;
};

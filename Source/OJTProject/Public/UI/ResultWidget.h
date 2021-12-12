// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetResultMessage(bool bIsWin, int inTurnCount);

protected:
	UPROPERTY()
	class UTextBlock* GameResultText;
	
	UPROPERTY()
	class UTextBlock* FinalTurnText;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurnManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayTurnDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextTurnDelegate);

/**
 * 
 */
UCLASS()
class OJTPROJECT_API UTurnManager : public UObject
{
	GENERATED_BODY()
	
public:
	FOnPlayTurnDelegate OnPlayTurnDelegate;
	FOnNextTurnDelegate OnNextTurnDelegate;

	void PlayTurnDelegate();
	void NextTurnDelegate();
};

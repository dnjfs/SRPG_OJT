// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleMode.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ABattleMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABattleMode();

protected:
	virtual void BeginPlay() override;
};

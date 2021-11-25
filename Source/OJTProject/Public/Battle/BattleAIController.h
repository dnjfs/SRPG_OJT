// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BattleAIController.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ABattleAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABattleAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	void MoveCharacter(FVector TargetLocation);

private:
};
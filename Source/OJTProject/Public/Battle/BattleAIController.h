// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Battle/BattleCharacter.h"
#include "BattleAIController.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ABattleAIController : public AAIController
{
	GENERATED_BODY()

	enum class EBehaviorType
	{
		NONE,
		MOVE,
		ATTACK,
		SKILL,
	};
	
public:
	ABattleAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void SetEndAttackDelegate();
	void MoveCharacter(TArray<FVector> TargetLocation);
	void AttackCharacter();

	bool GetNextDest();
	void AttackAnimation();

	void StartAIBehavior();
	void EndOfAIBehavior();

public:
	static const FName DestinationKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	TArray<FVector> DestArray;

	int32 DestIndex = -1;

	EBehaviorType BehaviorType;
	//ABattleCharacter* TargetCharacter;
};

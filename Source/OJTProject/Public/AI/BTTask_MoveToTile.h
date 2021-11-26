// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToTile.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API UBTTask_MoveToTile : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToTile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
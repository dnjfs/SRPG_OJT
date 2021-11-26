// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToTile.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToTile::UBTTask_MoveToTile()
{
	NodeName = TEXT("MoveToTile");
}

EBTNodeResult::Type UBTTask_MoveToTile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControllingPawn == nullptr)
		return EBTNodeResult::Failed;

	if(!Cast<ABattleAIController>(OwnerComp.GetAIOwner())->GetNextDest())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finish Move"));
		return EBTNodeResult::Failed;
		//return EBTNodeResult::Aborted;
	}

	return EBTNodeResult::Succeeded;
}
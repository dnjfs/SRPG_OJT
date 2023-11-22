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
		Cast<ABattleAIController>(OwnerComp.GetAIOwner())->PostMovement(); //공격 애니메이션 실행

		//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //이동 완료, 태스크 종료
		return EBTNodeResult::Failed; //태스크를 수행했지만 실패
		//return EBTNodeResult::Aborted; //태스크 실행 중에 중단
	}

	return EBTNodeResult::Succeeded; //태스크를 성공적으로 수행
}
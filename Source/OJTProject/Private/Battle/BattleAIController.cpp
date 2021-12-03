// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleAIController.h"
#include "Map/MapGameInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABattleAIController::DestinationKey(TEXT("Destination"));

ABattleAIController::ABattleAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_BattleCharacter.BB_BattleCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABattleAIController: BBObject faliled"));
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_BattleCharacter.BT_BattleCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABattleAIController: BTObject faliled"));
	}
}


void ABattleAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//UE_LOG(LogTemp, Warning, TEXT("ABattleAIController::OnPossess : %s"), *InPawn->GetName());
}

void ABattleAIController::OnUnPossess()
{
	//UE_LOG(LogTemp, Warning, TEXT("ABattleAIController::OnUnPossess : %s"), *GetPawn()->GetName());
	Super::OnUnPossess();
}

void ABattleAIController::MoveCharacter(TArray<FVector> TargetLocation)
{
	StartAIBehavior();
	DestArray = TargetLocation;
	if (UseBlackboard(BBAsset, Blackboard))
	{
		//Blackboard->SetValueAsVector(DestinationKey, DestArray[DestIndex]);
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}

}

bool ABattleAIController::GetNextDest()
{
	if(++DestIndex < DestArray.Num())
	{
		Blackboard->SetValueAsVector(DestinationKey, DestArray[DestIndex]);
		return true;
	}

	//이동 완료 상태
	//공격을 할 필요가 없다면 비헤이비어 트리 종료
	//AI 플레이어에 타겟 액터를 위크포인트로 잡고있기? nullptr이 아니면 타겟이 있는 것이므로 공격..?
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	DestIndex = -1;
	DestArray.Empty();

	EndOfAIBehavior();
	return false;
}

void ABattleAIController::StartAIBehavior()
{
	UE_LOG(LogTemp, Warning, TEXT("-------------------- Turn Delegate Start --------------------"));
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->PlayTurnDelegate();
}
void ABattleAIController::EndOfAIBehavior()
{
	UE_LOG(LogTemp, Warning, TEXT("-------------------- Turn Delegate End --------------------"));
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->NextTurnDelegate();
}
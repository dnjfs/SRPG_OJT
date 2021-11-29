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

	//�ݺ��� �Ἥ ���� ����Ʈ��� �����̱�
	//MoveToLocation�� �Ϸ� ���� �� Ÿ�̹��� �˾ƾ���
	//for(FVector target : TargetLocation)
	//{
	//	if(EPathFollowingRequestResult::RequestSuccessful == MoveToLocation(target, 0, false)) //Task �Ǵ� Behavior Tree�� ����
	//	{
	//		FVector AILoc = GetPawn()->GetActorLocation();
	//		UE_LOG(LogTemp, Warning, TEXT("%s Move To (%f %f) -> (%f, %f)"), *GetPawn()->GetName(), AILoc.X, AILoc.Y, target.X, target.Y);
	//	}
	//}

}

bool ABattleAIController::GetNextDest()
{
	if(++DestIndex < DestArray.Num())
	{
		Blackboard->SetValueAsVector(DestinationKey, DestArray[DestIndex]);
		return true;
	}

	//�����̺�� Ʈ�� ����
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleAIController.h"
#include "Battle/PlayerAnimInstance.h"
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

void ABattleAIController::SetEndAttackDelegate()
{
	Cast<UPlayerAnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance())->OnEndAttack.AddLambda([this]() {
		BehaviorType = EBehaviorType::NONE; //TargetCharacter = nullptr;
		Cast<ABattleCharacter>(GetCharacter())->SetAttackState();
		EndOfAIBehavior();
	});
}

void ABattleAIController::MoveCharacter(TArray<FVector> TargetLocation)
{
	DestArray = TargetLocation;
	StartAIBehavior();
}

void ABattleAIController::AttackCharacter()
{
	BehaviorType = EBehaviorType::ATTACK;
	//this->TargetCharacter = inTargetCharacter; //캐릭터에서 직접 가지고 있음
}

bool ABattleAIController::GetNextDest()
{
	if(++DestIndex < DestArray.Num())
	{
		Blackboard->SetValueAsVector(DestinationKey, DestArray[DestIndex]);
		return true;
	}
	DestIndex = -1;
	DestArray.Empty();

	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	return false;
}

void ABattleAIController::PostMovement()
{
	//이동 완료 상태

	if (BehaviorType == EBehaviorType::ATTACK) //공격하는 행동일 경우
	{
		//현재 상태에 따라 공격/스킬 애니메이션 실행
		Cast<ABattleCharacter>(GetCharacter())->PlayAnimationMontage();
	}
	else //공격을 할 필요가 없다면 비헤이비어 트리 종료
	{
		EndOfAIBehavior();
	}
}

void ABattleAIController::StartAIBehavior()
{
	UE_LOG(LogTemp, Warning, TEXT("-------------------- Turn Delegate Start --------------------"));
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->PlayTurnDelegate();

	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
void ABattleAIController::EndOfAIBehavior()
{
	UE_LOG(LogTemp, Warning, TEXT("-------------------- Turn Delegate End --------------------"));
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->NextTurnDelegate();
}
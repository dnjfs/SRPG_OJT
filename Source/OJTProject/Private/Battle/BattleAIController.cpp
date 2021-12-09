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
	//this->TargetCharacter = inTargetCharacter; //ĳ���Ϳ��� ���� ������ ����
}
void ABattleAIController::SkillCharacter()
{
	BehaviorType = EBehaviorType::SKILL;
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
	//�̵� �Ϸ� ����
	
	if (BehaviorType == EBehaviorType::ATTACK) //�����ϴ� �ൿ�� ���
	{
		//Attack ���·� �����ϰ� �ִϸ��̼� ����
		Cast<ABattleCharacter>(GetCharacter())->SetAttackState();
	}
	else if (BehaviorType == EBehaviorType::SKILL) //��ų�� ���� �ൿ�� ���
	{
		Cast<ABattleCharacter>(GetCharacter())->SetSkillState();
	}
	else //������ �� �ʿ䰡 ���ٸ� �����̺�� Ʈ�� ����
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
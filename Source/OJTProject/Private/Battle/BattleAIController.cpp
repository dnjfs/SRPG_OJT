// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleAIController.h"
#include "AITypes.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"

ABattleAIController::ABattleAIController()
{

}


void ABattleAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("ABattleAIController::OnPossess : %s"), *InPawn->GetName());
}

void ABattleAIController::OnUnPossess()
{
	UE_LOG(LogTemp, Warning, TEXT("ABattleAIController::OnUnPossess : %s"), *GetPawn()->GetName());
	Super::OnUnPossess();
}

void ABattleAIController::MoveCharacter(FVector TargetLocation)
{
	MoveToLocation(TargetLocation, 0, false);
	UE_LOG(LogTemp, Warning, TEXT("%s Move To (%f, %f)"), *GetPawn()->GetName(), TargetLocation.X, TargetLocation.Y);
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetLocation);
}
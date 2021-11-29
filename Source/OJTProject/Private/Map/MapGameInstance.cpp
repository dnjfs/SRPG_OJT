// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapGameInstance.h"

UMapGameInstance::UMapGameInstance()
{
	FString LevelDataPath = TEXT("DataTable'/Game/Data/Level.Level'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LEVEL(*LevelDataPath);
	if(DT_LEVEL.Succeeded())
	{
		LevelTable = DT_LEVEL.Object;
	}

	TurnManager = NewObject<UTurnManager>();
}

void UMapGameInstance::Init()
{
	Super::Init();
}

FOJTLevelData* UMapGameInstance::GetLevelData(int32 Level)
{
	return LevelTable->FindRow<FOJTLevelData>(*FString::FromInt(Level), TEXT(""));
}

UTurnManager* UMapGameInstance::GetTurnManagerInstance()
{
	return TurnManager;
}
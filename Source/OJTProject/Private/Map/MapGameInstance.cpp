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

	FString CharacterDataPath = TEXT("DataTable'/Game/Data/Character.Character'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER(*CharacterDataPath);
	if (DT_CHARACTER.Succeeded())
	{
		CharacterTable = DT_CHARACTER.Object;
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

FCharacterTable* UMapGameInstance::GetCharcaterData(CharacterType CType)
{
	TArray<FCharacterTable*> datas;
	CharacterTable->GetAllRows<FCharacterTable>(TEXT(""), datas);

	for (auto tChar : datas)
	{
		if(tChar->CharType == CType)
			return tChar;
	}
	return nullptr;
}

UTurnManager* UMapGameInstance::GetTurnManagerInstance()
{
	return TurnManager;
}
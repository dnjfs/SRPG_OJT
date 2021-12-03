// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Map/TurnManager.h"
#include "MapGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FOJTLevelData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FOJTLevelData() : Level(1), column(5), row(4) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 column;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 row;
};


UENUM()
enum class ECharacterType
{
	NONE,
	PLAYER1,
	PLAYER2,
	ENEMY,
};

USTRUCT(BlueprintType)
struct FCharacterTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCharacterTable() : CharType(ECharacterType::NONE), SKChar(nullptr), AMChar(nullptr) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		ECharacterType CharType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		USkeletalMesh* SKChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TSubclassOf<UAnimInstance> AIChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UAnimMontage* AMChar;
};

/**
 * 
 */
UCLASS()
class OJTPROJECT_API UMapGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMapGameInstance();
	virtual void Init() override;

	FOJTLevelData* GetLevelData(int32 Level);
	FCharacterTable* GetCharcaterData(ECharacterType CType);

	UTurnManager* GetTurnManagerInstance();
		
private:
	UPROPERTY()
	class UDataTable* LevelTable;

	UPROPERTY()
	class UDataTable* CharacterTable;

	UPROPERTY()
	class UTurnManager* TurnManager;
};

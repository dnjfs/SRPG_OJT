// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
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
		
private:
	UPROPERTY()
	class UDataTable* LevelTable;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OJTProject.h"
#include "Map/TileCell.h"
#include "GameFramework/GameStateBase.h"
#include "BattleState.generated.h"

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ABattleState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	~ABattleState() { TileMap.Empty(); }

	virtual void BeginPlay() override;
	void SpawnTiles();

	UPROPERTY()
	TArray<ATileCell*> TileMap;

protected:
	UPROPERTY(Transient)
	int32 BattleLevel;
	
	UPROPERTY(Transient)
	int32 BattleRow;
	
	UPROPERTY(Transient)
	int32 BattleColumn;

private:
	struct FOJTLevelData* CurrentLevelData;
};

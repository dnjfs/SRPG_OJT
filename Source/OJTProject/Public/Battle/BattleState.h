// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OJTProject.h"
#include "Map/TileCell.h"
#include "Battle/BattleCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "BattleState.generated.h"

//USTRUCT()
//struct OJTPROJECT_API FTileRow
//{
//	TArray<ATileCell*> TileLine;
//};

/**
 * 
 */
UCLASS()
class OJTPROJECT_API ABattleState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	~ABattleState() { TileMap.Empty(); } //π∫∞° ¥ı «ÿ¡‡æﬂ «“ ∞Õ ∞∞¿Ω;

	virtual void BeginPlay() override;
	void SpawnTiles();
	void SpawnCharacter();
	
	UFUNCTION()
	void ClickTile(AActor* aActor);

	int CalcTileIndex(int inTileID);

	void NextTurn();

	void FindRoute(int StartTile, int EndTile, TArray<FVector>& Route);

	UPROPERTY()
	TArray<ATileCell*> TileMap;
	//UPROPERTY()
	//TArray<FTileRow> TileMap2;

	UPROPERTY()
	TArray<ABattleCharacter*> Player;
	UPROPERTY()
	TArray<ABattleCharacter*> Enemy;

protected:
	UPROPERTY(Transient)
	int32 BattleLevel = -1;
	
	UPROPERTY(Transient)
	int32 BattleRow = -1;
	
	UPROPERTY(Transient)
	int32 BattleColumn = -1;

	UPROPERTY(Transient)
	int32 CurrentTile = -1;

private:
	struct FOJTLevelData* CurrentLevelData;
	int32 CurrentTurn = -1;
};

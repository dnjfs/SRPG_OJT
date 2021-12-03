// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OJTProject.h"
#include "Map/TileCell.h"
#include "Battle/BattleCharacter.h"
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
	~ABattleState() { TileMap.Empty(); } //뭔가 더 해줘야 할 것 같음;

	virtual void BeginPlay() override;
	void SpawnTiles();
	void SpawnCharacter();
	
	UFUNCTION()
	void ClickTile(AActor* aActor);

	int IDToIndex(int inTileID);

	UFUNCTION()
	void PlayTurn();
	UFUNCTION()
	void NextTurn();

	void FindRoute(int StartTile, int EndTile, TArray<FVector>& Route);
	
	void MoveTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter);

	UPROPERTY()
	TArray<ATileCell*> TileMap;
	UPROPERTY()
	TArray<int32> CharacterTile; //R x C 배열, 0은 비어있고 1은 플레이어, 2는 적군

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

private:
	struct FOJTLevelData* CurrentLevelData;

	int32 CurrentTileID = -1;
	int32 CurrentTurn = -1;

	bool bIsRunBehavior = false;
	bool bIsPlayerTurn = true;
};

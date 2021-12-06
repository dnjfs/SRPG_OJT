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

	void ClearTileSM(); //타일 스태틱메시 초기화
	void AvailableTileSM(int PlayerID); //선택한 캐릭터가 이동 가능한 타일 표시
	void AttackTileSM(int TargetID); //타겟에서 공격 가능한 타일 표시

	int IDToIndex(int inTileID);

	UFUNCTION()
	void PlayTurn();
	UFUNCTION()
	void NextTurn();

	void FindRoute(int StartTile, int EndTile, TArray<FVector>& Route);
	
	void MoveTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter);
	void AttackTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter, ABattleCharacter* TargetCharacter);

	UPROPERTY()
	TArray<ATileCell*> TileMap;
	UPROPERTY()
	TArray<ABattleCharacter*> CharacterTile; //R x C 배열, 캐릭터가 없으면 nullptr

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
	int32 AttackTileID = -1;
	int32 CurrentTurn = -1;

	bool bIsRunBehavior = false;
	bool bIsPlayerTurn = true;
};

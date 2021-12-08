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
	void AvailableTileSM(ABattleCharacter* CurrentPlayer); //선택한 캐릭터가 이동 가능한 타일 표시
	void AttackTileSM(ABattleCharacter* CurrentPlayer, int TargetID); //타겟에서 공격 가능한 타일 표시

	int IDToIndex(int inTileID);

	UFUNCTION()
	void PlayTurn();
	UFUNCTION()
	void NextTurn();

	void FindRoute(int StartTile, int EndTile, TArray<FVector>& Route);
	
	void MoveTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter);
	void AttackTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter, ABattleCharacter* TargetCharacter);
	void SkillTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter, ABattleCharacter* TargetCharacter);

	UFUNCTION()
	void DeleteCharacter(int TileLocID); //BattleCharacter에서 델리게이트로 호출

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

	int32 PlayerTileID = -1; //현재 턴의 플레이어가 서있는 타일
	int32 CurrentTileID = -1; //플레이어 턴에 선택한 타일
	int32 AttackTileID = -1; //공격할 대상의 타일

	int32 CurrentTurn = -1; //턴 부여용
	int32 TurnCount = 0; //플레이어의 턴 카운팅

	bool bIsRunBehavior = false;
	bool bIsPlayerTurn = true;
};

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
	~ABattleState() { TileMap.Empty(); } //���� �� ����� �� �� ����;

	virtual void BeginPlay() override;
	void SpawnTiles();
	void SpawnCharacter();
	
	UFUNCTION()
	void ClickTile(AActor* aActor);

	void ClearTileSM(); //Ÿ�� ����ƽ�޽� �ʱ�ȭ
	void AvailableTileSM(ABattleCharacter* CurrentPlayer); //������ ĳ���Ͱ� �̵� ������ Ÿ�� ǥ��
	void AttackTileSM(ABattleCharacter* CurrentPlayer, int TargetID); //Ÿ�ٿ��� ���� ������ Ÿ�� ǥ��

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
	void DeleteCharacter(int TileLocID); //BattleCharacter���� ��������Ʈ�� ȣ��

	UPROPERTY()
	TArray<ATileCell*> TileMap;
	UPROPERTY()
	TArray<ABattleCharacter*> CharacterTile; //R x C �迭, ĳ���Ͱ� ������ nullptr

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

	int32 PlayerTileID = -1; //���� ���� �÷��̾ ���ִ� Ÿ��
	int32 CurrentTileID = -1; //�÷��̾� �Ͽ� ������ Ÿ��
	int32 AttackTileID = -1; //������ ����� Ÿ��

	int32 CurrentTurn = -1; //�� �ο���
	int32 TurnCount = 0; //�÷��̾��� �� ī����

	bool bIsRunBehavior = false;
	bool bIsPlayerTurn = true;
};

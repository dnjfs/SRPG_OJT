// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleState.h"
#include "Map/MapGameInstance.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine/StaticMeshActor.h"

void ABattleState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABattleState::BeginPlay"));

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); //���� ���� �̸� �ҷ�����
	BattleLevel = FCString::Atoi(*(LevelName.Right(1)));

	SpawnTiles(); //Ÿ�� ����
}

void ABattleState::SpawnTiles()
{
	auto GameInst = Cast<UMapGameInstance>(GetGameInstance());
	CurrentLevelData = GameInst->GetLevelData(BattleLevel);

	BattleColumn = CurrentLevelData->column;
	BattleRow = CurrentLevelData->row;

	UE_LOG(LogTemp, Warning, TEXT("Level%d - Row: %d, Column: %d"), BattleLevel, BattleRow, BattleColumn);

	AStaticMeshActor* Floor = nullptr;
	for (const auto& entity : TActorRange<AStaticMeshActor>(GetWorld())) //Floor ã��
	{
		if(*entity->GetName() == FString("Floor"))
			Floor = entity;
		//UE_LOG(LogTemp, Warning, TEXT("Loaded UStaticMesh : %s"), *entity->GetName());
	}
	FVector floorLoc = Floor->GetActorTransform().GetLocation() + FVector(-100.0 * (BattleColumn-1), -100.0 * (BattleRow -1), 50); //�»�� ����
	UE_LOG(LogTemp, Warning, TEXT("Floor - X: %f, Y: %f"), floorLoc.X, floorLoc.Y);


	UWorld* world = GetWorld();
	int tID = 0;
	for(int y = 0; y < BattleRow; ++y)
	{
		for (int x = 0; x < BattleColumn; ++x)
		{
			//world->SpawnActor<ATileCell>(floorLoc, FRotator::ZeroRotator);
			ATileCell* tile = world->SpawnActor<ATileCell>(floorLoc + FVector(200 * x, 200 * y, 0), FRotator::ZeroRotator);
			tile->SetTileID(tID++); //ID ����
			tile->AttachToActor(Floor, FAttachmentTransformRules::KeepWorldTransform); //�θ� ����
			TileMap.Add(tile); //���� ����
			//TileMap.Last()->SetTileID(tID++);
		}
		tID /= 10;
		tID++;
		tID *= 10;
	}
}

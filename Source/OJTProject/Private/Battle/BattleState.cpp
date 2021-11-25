// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleState.h"
#include "Battle/BattleAIController.h"
#include "Map/MapGameInstance.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine/StaticMeshActor.h"

void ABattleState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABattleState::BeginPlay"));

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); //현재 레벨 이름 불러오기
	BattleLevel = FCString::Atoi(*(LevelName.Right(1))); //레벨 이름의 가장 오른쪽 번호 가져오기

	CurrentTile = -1; //선택된 타일이 없음
	CurrentTurn = 0; //0번째부터 시작

	SpawnTiles(); //타일 스폰
	SpawnCharacter(); //캐릭터 스폰
	//OnClicked.AddDynamic(this, &ABattleState::PrintName);
}

void ABattleState::SpawnTiles()
{
	auto GameInst = Cast<UMapGameInstance>(GetGameInstance());
	CurrentLevelData = GameInst->GetLevelData(BattleLevel);

	BattleColumn = CurrentLevelData->column;
	BattleRow = CurrentLevelData->row;

	UE_LOG(LogTemp, Warning, TEXT("Level%d - Row: %d, Column: %d"), BattleLevel, BattleRow, BattleColumn);

	AStaticMeshActor* Floor = nullptr;
	for (const auto& entity : TActorRange<AStaticMeshActor>(GetWorld())) //Floor 찾기
	{
		if(*entity->GetName() == FString("Floor"))
			Floor = entity;
		//UE_LOG(LogTemp, Warning, TEXT("Loaded UStaticMesh : %s"), *entity->GetName());
	}
	FVector floorLoc = Floor->GetActorTransform().GetLocation() + FVector(-100.0 * (BattleColumn-1), -100.0 * (BattleRow -1), 50); //좌상단 기준
	UE_LOG(LogTemp, Warning, TEXT("Floor - X: %f, Y: %f"), floorLoc.X, floorLoc.Y);

	UWorld* world = GetWorld();
	int tID = 0;
	for(int y = 0; y < BattleRow; ++y)
	{
		for (int x = 0; x < BattleColumn; ++x)
		{
			//world->SpawnActor<ATileCell>(floorLoc, FRotator::ZeroRotator);
			ATileCell* tile = world->SpawnActor<ATileCell>(floorLoc + FVector(200 * x, 200 * y, 0), FRotator::ZeroRotator);
			tile->SetTileID(tID++); //ID 지정
			tile->AttachToActor(Floor, FAttachmentTransformRules::KeepWorldTransform); //부모 설정
			tile->SetActorLabel("Tile" + FString::FromInt(y) + FString::FromInt(x)); //타일 이름 변경
			tile->OnTileSelecedDelegate.AddDynamic(this, &ABattleState::ClickTile); //델리게이트 연결
			//tile->OnClicked.AddDynamic(this, &ABattleState::PrintName);
			TileMap.Add(tile); //액터 생성
		}
		tID /= 10;
		tID++;
		tID *= 10;
	}
	/*
	UWorld* world = GetWorld();
	int tID = 0;
	for (int y = 0; y < BattleRow; ++y)
	{
		TileRow tileRow;
		for (int x = 0; x < BattleColumn; ++x)
		{
			//world->SpawnActor<ATileCell>(floorLoc, FRotator::ZeroRotator);
			ATileCell* tile = world->SpawnActor<ATileCell>(floorLoc + FVector(200 * x, 200 * y, 0), FRotator::ZeroRotator);
			tile->SetTileID(tID++); //ID 지정
			tile->AttachToActor(Floor, FAttachmentTransformRules::KeepWorldTransform); //부모 설정
			tile->SetActorLabel("Tile" + FString::FromInt(y) + FString::FromInt(x)); //타일 이름 변경
			tile->OnTileSelecedDelegate.AddDynamic(this, &ABattleState::PrintName); //델리게이트 연결
			//tile->OnClicked.AddDynamic(this, &ABattleState::PrintName);
			tileRow.TileLine.Add(tile);
		}
		TileMap2.Add(tileRow); //액터 생성
		tID /= 10;
		tID++;
		tID *= 10;
	}
	*/
}

void ABattleState::SpawnCharacter()
{
	int n = 4; //생성할 캐릭터 수
	for(int i = 0; i < n; i++) //캐릭터 생성
	{
		int32 loc = BattleColumn * i;
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ABattleCharacter>(TileMap[loc]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player"+FString::FromInt(i));
		BTChar->SetTileLocation(TileMap[loc]->GetTileID());
		Player.Add(BTChar);
		UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}

	for (int i = 0; i < n; i++) //적 생성
	{
		int32 loc = (BattleColumn - 1) + (BattleColumn * i);
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ABattleCharacter>(TileMap[loc]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator(0, 180, 0));
		BTChar->SetActorLabel("Enemy"+FString::FromInt(i));
		BTChar->SetTileLocation(TileMap[loc]->GetTileID());
		Enemy.Add(BTChar);
		UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}
}

void ABattleState::ClickTile(AActor* aActor)
{
	int selected = Cast<ATileCell>(aActor)->GetTileID();
	if(CurrentTile == -1) //선택된 타일이 없는 경우
	{
		if(selected != Player[CurrentTurn]->GetTileLocation()) //플레이어가 서있는 곳을 클릭해야 선택됨
		{
			UE_LOG(LogTemp, Warning, TEXT("Not on Character : %d"), selected);
			return;
		}

		CurrentTile = selected;
		Cast<ATileCell>(aActor)->ChangeSMSelected();
		UE_LOG(LogTemp, Warning, TEXT("TileSelect: %d"), CurrentTile);
	}
	else
	{
		int TargetTile = CalcTileIndex(CurrentTile);
		TileMap[TargetTile]->ChangeSMIdle();
		UE_LOG(LogTemp, Warning, TEXT("TileRelease: %d -> %d"), CurrentTile, selected);

		Cast<ABattleAIController>(Player[CurrentTurn]->GetController())->MoveCharacter(TileMap[TargetTile]->GetActorLocation()); //목적지 정하여 움직이도록 구현

		CurrentTile = -1;
		CurrentTurn++;
		if(CurrentTurn >= 4)
			CurrentTurn = 0;
	}
	//UE_LOG(LogTemp, Warning, TEXT("TileSelect: %s"), *aActor->GetName());
}

int ABattleState::CalcTileIndex(int inTileID)
{
	return (inTileID/10)*BattleColumn + (inTileID%10);
}
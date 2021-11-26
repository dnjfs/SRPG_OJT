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

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); //���� ���� �̸� �ҷ�����
	BattleLevel = FCString::Atoi(*(LevelName.Right(1))); //���� �̸��� ���� ������ ��ȣ ��������

	CurrentTile = -1; //���õ� Ÿ���� ����
	CurrentTurn = 0; //0��°���� ����

	SpawnTiles(); //Ÿ�� ����
	SpawnCharacter(); //ĳ���� ����
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
			tile->SetActorLabel("Tile" + FString::FromInt(y) + FString::FromInt(x)); //Ÿ�� �̸� ����
			tile->OnTileSelecedDelegate.AddDynamic(this, &ABattleState::ClickTile); //��������Ʈ ����
			//tile->OnClicked.AddDynamic(this, &ABattleState::PrintName);
			TileMap.Add(tile); //���� ����
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
			tile->SetTileID(tID++); //ID ����
			tile->AttachToActor(Floor, FAttachmentTransformRules::KeepWorldTransform); //�θ� ����
			tile->SetActorLabel("Tile" + FString::FromInt(y) + FString::FromInt(x)); //Ÿ�� �̸� ����
			tile->OnTileSelecedDelegate.AddDynamic(this, &ABattleState::PrintName); //��������Ʈ ����
			//tile->OnClicked.AddDynamic(this, &ABattleState::PrintName);
			tileRow.TileLine.Add(tile);
		}
		TileMap2.Add(tileRow); //���� ����
		tID /= 10;
		tID++;
		tID *= 10;
	}
	*/
}

void ABattleState::SpawnCharacter()
{
	int n = 4; //������ ĳ���� ��
	for(int i = 0; i < n; i++) //ĳ���� ����
	{
		int32 loc = BattleColumn * i;
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ABattleCharacter>(TileMap[loc]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player"+FString::FromInt(i));
		BTChar->SetTileLocation(TileMap[loc]->GetTileID());
		Player.Add(BTChar);
		//UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}

	for (int i = 0; i < n; i++) //�� ����
	{
		int32 loc = (BattleColumn - 1) + (BattleColumn * i);
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ABattleCharacter>(TileMap[loc]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator(0, 180, 0));
		BTChar->SetActorLabel("Enemy"+FString::FromInt(i));
		BTChar->SetTileLocation(TileMap[loc]->GetTileID());
		Enemy.Add(BTChar);
		//UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}
}

void ABattleState::ClickTile(AActor* aActor)
{
	int selected = Cast<ATileCell>(aActor)->GetTileID();
	if(CurrentTile == -1) //���õ� Ÿ���� ���� ���
	{
		if(selected != Player[CurrentTurn]->GetTileLocation()) //�÷��̾ ���ִ� ���� Ŭ���ؾ� ���õ�
		{
			UE_LOG(LogTemp, Warning, TEXT("Not on Character%d : %d"), CurrentTurn, selected);
			return;
		}

		CurrentTile = selected;
		TileMap[CalcTileIndex(CurrentTile)]->ChangeSMSelected(); //Cast<ATileCell>(aActor)->ChangeSMSelected();
		UE_LOG(LogTemp, Warning, TEXT("TileSelect: %d"), CurrentTile);
	}
	else
	{
		TileMap[CalcTileIndex(CurrentTile)]->ChangeSMIdle();
		UE_LOG(LogTemp, Warning, TEXT("TileRelease: %d -> %d"), CurrentTile, selected);

		if (selected == Player[CurrentTurn]->GetTileLocation()) //�÷��̾ ���ִ� ���� Ŭ���ؾ� ���õ�
		{
			CurrentTile = -1;
			UE_LOG(LogTemp, Warning, TEXT("Not on Character : %d"), selected);
			return;
		}
		int TargetTile = CalcTileIndex(selected);
		TArray<FVector> ArrVec;
		FindRoute(Player[CurrentTurn]->GetTileLocation(), selected, ArrVec);
		//ArrVec.Add(TileMap[TargetTile+5]->GetActorLocation());
		//ArrVec.Add(TileMap[TargetTile]->GetActorLocation());
		Cast<ABattleAIController>(Player[CurrentTurn]->GetController())->MoveCharacter(ArrVec); //���İ����� ���� ����Ʈ �����ؾ���

		Player[CurrentTurn]->SetTileLocation(selected);

		CurrentTile = -1;
		NextTurn();
	}
}

int ABattleState::CalcTileIndex(int inTileID)
{
	return (inTileID/10)*BattleColumn + (inTileID%10);
}

void ABattleState::NextTurn()
{
	if (++CurrentTurn >= 4) //�ϴ��� �÷��̾� ĳ���͸� ����
		CurrentTurn = 0;
}

//BFS�� ��ã��
void ABattleState::FindRoute(int StartTile, int EndTile, TArray<FVector>& Route)
{
	int visited[100];
	int parent[100] = {0};
	for(int i = 0; i < 100; i++)
		visited[i] = 100;

	TQueue<int> q;
	q.Enqueue(StartTile);
	visited[StartTile] = 0;
	parent[StartTile] = -1;

	int v = -1;
	while (!q.IsEmpty())
	{
		q.Dequeue(v);

		if(v == EndTile)
			break;

		int t = v-1; //��
		if (v%10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v]+1;
			parent[t] = v;
			q.Enqueue(t);
		}
		t = v+1; //��
		if (v%10 < BattleColumn-1 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			parent[t] = v;
			q.Enqueue(t);
		}

		t = v-10; //��
		if (v/10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			parent[t] = v;
			q.Enqueue(t);
		}
		t = v+10; //��
		if (v/10 < BattleRow-1 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			parent[t] = v;
			q.Enqueue(t);
		}
	}
	
	TArray<int> ReverseRoute;
	while (v != -1)
	{
		ReverseRoute.Add(v);
		v = parent[v];
		UE_LOG(LogTemp, Warning, TEXT("Now V: %d"), v);
	}

	while (ReverseRoute.Num() != 0)
	{
		Route.Add(TileMap[CalcTileIndex(ReverseRoute.Last())]->GetActorLocation());
		ReverseRoute.RemoveAt(ReverseRoute.Num()-1); //������ ���� ����
	}

	//Route�� �� ù��°�� ������ġ�� ����Ǿ� �־� ������ �ɵ�
	for (auto vec : Route)
	{
		UE_LOG(LogTemp, Warning, TEXT("(%f %f)"), vec.X, vec.Y);
	}
}
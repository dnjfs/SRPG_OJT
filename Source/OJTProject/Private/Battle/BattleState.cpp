// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleState.h"
#include "Battle/BattleAIController.h"
#include "Map/MapGameInstance.h"
#include "Map/TurnManager.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine/StaticMeshActor.h"

void ABattleState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABattleState::BeginPlay"));

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); //���� ���� �̸� �ҷ�����
	BattleLevel = FCString::Atoi(*(LevelName.Right(1))); //���� �̸��� ���� ������ ��ȣ ��������

	SpawnTiles(); //Ÿ�� ����
	SpawnCharacter(); //ĳ���� ����

	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnPlayTurnDelegate.AddDynamic(this, &ABattleState::PlayTurn);
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnNextTurnDelegate.AddDynamic(this, &ABattleState::NextTurn);

	NextTurn(); //ù �� ����
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
			ATileCell* tile = world->SpawnActor<ATileCell>(floorLoc + FVector(200 * x, 200 * y, 0), FRotator::ZeroRotator);
			tile->SetTileID(tID++); //ID ����
			tile->AttachToActor(Floor, FAttachmentTransformRules::KeepWorldTransform); //�θ� ����
			tile->SetActorLabel("Tile" + FString::FromInt(y) + FString::FromInt(x)); //Ÿ�� �̸� ����
			tile->OnTileSelectedDelegate.AddDynamic(this, &ABattleState::ClickTile); //��������Ʈ ����
			TileMap.Add(tile); //���� ����
		}
		tID /= 10;
		tID++;
		tID *= 10;
	}

	CharacterTile.Init(0, BattleRow * BattleColumn);
}

void ABattleState::SpawnCharacter()
{
	int n = 4; //������ ĳ���� ��
	for(int i = 0; i < n; i++) //ĳ���� ����
	{
		int32 locIndex = BattleColumn * i;
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ABattleCharacter>(TileMap[locIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player"+FString::FromInt(i));
		BTChar->SetTileLocationID(TileMap[locIndex]->GetTileID());
		BTChar->SetPlayerCharacter(true);
		Player.Add(BTChar);
		CharacterTile[locIndex] = 1;
		//UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}

	for (int i = 0; i < n; i++) //�� ����
	{
		int32 locIndex = (BattleColumn - 1) + (BattleColumn * i);
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ABattleCharacter>(TileMap[locIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator(0, 180, 0));
		BTChar->SetActorLabel("Enemy"+FString::FromInt(i));
		BTChar->SetTileLocationID(TileMap[locIndex]->GetTileID());
		Enemy.Add(BTChar);
		CharacterTile[locIndex] = 2;
		//UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}
}

void ABattleState::ClickTile(AActor* aActor)
{
	if(bIsRunBehavior) return; //�ൿ���� ��� Ŭ�� �Ұ�

	int selectedID = Cast<ATileCell>(aActor)->GetTileID();
	if(CurrentTileID == -1) //���õ� Ÿ���� ���� ���
	{
		if(selectedID != Player[CurrentTurn]->GetTileLocationID()) //�÷��̾ ���ִ� ���� Ŭ���ؾ� ���õ�
		{
			UE_LOG(LogTemp, Warning, TEXT("Not on Character%d : %d"), CurrentTurn, selectedID);
			return;
		}

		CurrentTileID = selectedID;
		TileMap[IDToIndex(CurrentTileID)]->ChangeTileSM(ETileType::Selected); //Cast<ATileCell>(aActor)->ChangeSMSelected();
		UE_LOG(LogTemp, Warning, TEXT("TileSelect: %d"), CurrentTileID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TileRelease: %d -> %d"), CurrentTileID, selectedID);

		if (selectedID == Player[CurrentTurn]->GetTileLocationID()) //�÷��̾ ���ִ� ���� Ŭ���� ���
		{
			TileMap[IDToIndex(CurrentTileID)]->ChangeTileSM(ETileType::Current);
			CurrentTileID = -1;
			UE_LOG(LogTemp, Warning, TEXT("Cancle Move : %d"), selectedID);
			return;
		}

		//int targetTileIndex = IDToIndex(selected);
		if (CharacterTile[IDToIndex(selectedID)] != 0) //�ٸ� �÷��̾ ���ִ� ���� Ŭ���� ���
		{
			UE_LOG(LogTemp, Warning, TEXT("%d Tile is Occupied"), selectedID);
			return;
		}
		//���� Ŭ������ �� �����ϵ��� �����ؾ���


		MoveTile(CurrentTileID, selectedID, Player);

		CurrentTileID = -1;
	}
}

int ABattleState::IDToIndex(int inTileID)
{
	return (inTileID/10)*BattleColumn + (inTileID%10);
}

void ABattleState::PlayTurn()
{
	bIsRunBehavior = true; //�� ���� �� Ÿ�� Ŭ�� ��Ȱ��ȭ
}

void ABattleState::NextTurn()
{
	if (bIsPlayerTurn && ++CurrentTurn >= Player.Num()) //�÷��̾��� �� ��� ����
	{
		bIsPlayerTurn = false;
		CurrentTurn = 0;
	}
	else if(!bIsPlayerTurn && ++CurrentTurn >= Enemy.Num()) //������ �� ��� ����
	{
		bIsPlayerTurn = true;
		CurrentTurn = 0;
	}
	
	if(bIsPlayerTurn)
	{
		TileMap[IDToIndex(Player[CurrentTurn]->GetTileLocationID())]->ChangeTileSM(ETileType::Current);
	}
	else
	{
		TileMap[IDToIndex(Enemy[CurrentTurn]->GetTileLocationID())]->ChangeTileSM(ETileType::Current);
	}
		
	if(bIsPlayerTurn) //�÷��̾� ���� �� ��� �Է� �����ϵ���
	{
		bIsRunBehavior = false;
	}
	else //���� �� ����
	{
		//���� AI ����
		MoveTile(Enemy[CurrentTurn]->GetTileLocationID(), Enemy[CurrentTurn]->GetTileLocationID() - 1, Enemy);
	}
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
		Route.Add(TileMap[IDToIndex(ReverseRoute.Last())]->GetActorLocation());
		ReverseRoute.RemoveAt(ReverseRoute.Num()-1); //������ ���� ����
	}

	//Route�� �� ù��°�� ������ġ�� ����Ǿ� �־� ������ �ɵ�
	for (auto vec : Route)
	{
		UE_LOG(LogTemp, Warning, TEXT("(%f %f)"), vec.X, vec.Y);
	}
}

void ABattleState::MoveTile(int StartTileID, int EndTileID, TArray<ABattleCharacter*>& BCharacter)
{
	TArray<FVector> ArrVec;
	FindRoute(BCharacter[CurrentTurn]->GetTileLocationID(), EndTileID, ArrVec);

	Cast<ABattleAIController>(BCharacter[CurrentTurn]->GetController())->MoveCharacter(ArrVec); //���İ����� ���� ����Ʈ �����ؾ���

	CharacterTile[IDToIndex(EndTileID)] = CharacterTile[IDToIndex(StartTileID)]; //ĳ���� �̵�
	CharacterTile[IDToIndex(StartTileID)] = 0; //���� �ڸ��� ��������
	BCharacter[CurrentTurn]->SetTileLocationID(EndTileID);

	TileMap[IDToIndex(StartTileID)]->ChangeTileSM(ETileType::Idle);
}
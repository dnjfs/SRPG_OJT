// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleState.h"
#include "Battle/BattleAIController.h"
#include "Map/MapGameInstance.h"
#include "Map/TurnManager.h"

#include "Character/CharacterShort.h"
#include "Character/CharacterLong.h"
#include "Character/CharacterEnemy.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine/StaticMeshActor.h"

void ABattleState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABattleState::BeginPlay"));

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); //현재 레벨 이름 불러오기
	BattleLevel = FCString::Atoi(*(LevelName.Right(1))); //레벨 이름의 가장 오른쪽 번호 가져오기

	SpawnTiles(); //타일 스폰
	SpawnCharacter(); //캐릭터 스폰

	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnPlayTurnDelegate.AddDynamic(this, &ABattleState::PlayTurn);
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnNextTurnDelegate.AddDynamic(this, &ABattleState::NextTurn);

	NextTurn(); //첫 턴 시작
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
	//UE_LOG(LogTemp, Warning, TEXT("Floor - X: %f, Y: %f"), floorLoc.X, floorLoc.Y);

	UWorld* world = GetWorld();
	int tID = 0;
	for(int y = 0; y < BattleRow; ++y)
	{
		for (int x = 0; x < BattleColumn; ++x)
		{
			ATileCell* tile = world->SpawnActor<ATileCell>(floorLoc + FVector(200 * x, 200 * y, 0), FRotator::ZeroRotator);
			tile->SetTileID(tID++); //ID 지정
			tile->AttachToActor(Floor, FAttachmentTransformRules::KeepWorldTransform); //부모 설정
			tile->SetActorLabel("Tile" + FString::FromInt(y) + FString::FromInt(x)); //타일 이름 변경
			tile->OnTileSelectedDelegate.AddDynamic(this, &ABattleState::ClickTile); //델리게이트 연결
			TileMap.Add(tile); //액터 생성
		}
		tID /= 10;
		tID++;
		tID *= 10;
	}

	CharacterTile.Init(nullptr, BattleRow * BattleColumn);
}

void ABattleState::SpawnCharacter()
{
	int n = 2; //생성할 캐릭터 수
	for(int i = 0; i < n-1; i++) //근접 캐릭터 생성
	{
		int32 locIndex = BattleColumn * i;
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ACharacterShort>(TileMap[locIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player"+FString::FromInt(i));
		BTChar->SetTileLocationID(TileMap[locIndex]->GetTileID());
		BTChar->OnNotifyDeadDelegate.AddDynamic(this, &ABattleState::DeleteCharacter);
		Player.Add(BTChar);
		CharacterTile[locIndex] = BTChar;
		//UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}
	for (int i = 1; i < n; i++) //원거리 캐릭터 생성
	{
		int32 locIndex = BattleColumn * i;
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ACharacterLong>(TileMap[locIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player" + FString::FromInt(i));
		BTChar->SetTileLocationID(TileMap[locIndex]->GetTileID());
		BTChar->OnNotifyDeadDelegate.AddDynamic(this, &ABattleState::DeleteCharacter);
		Player.Add(BTChar);
		CharacterTile[locIndex] = BTChar;
	}

	for (int i = 0; i < n; i++) //적 생성
	{
		int32 locIndex = (BattleColumn - 1) + (BattleColumn * i);
		ABattleCharacter* BTChar = GetWorld()->SpawnActor<ACharacterEnemy>(TileMap[locIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator(0, 180, 0));
		BTChar->SetActorLabel("Enemy"+FString::FromInt(i));
		BTChar->SetTileLocationID(TileMap[locIndex]->GetTileID());
		BTChar->OnNotifyDeadDelegate.AddDynamic(this, &ABattleState::DeleteCharacter);
		Enemy.Add(BTChar);
		CharacterTile[locIndex] = BTChar;
		//UE_LOG(LogTemp, Warning, TEXT("Character Spawn: %s in %d -> x: %f, y: %f"), *BTChar->GetName(), BTChar->GetTileLocation(), BTChar->GetTransform().GetLocation().X, BTChar->GetTransform().GetLocation().Y);
	}
}

void ABattleState::ClickTile(AActor* aActor)
{
	if(bIsRunBehavior) return; //행동중인 경우 클릭 불가

	int selectedID = Cast<ATileCell>(aActor)->GetTileID();
	if (TileMap[IDToIndex(selectedID)]->GetTileType() == ETileType::Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't click this tile : %d"), selectedID);
		return;
	}

	if(CurrentTileID == -1) //선택된 타일이 없는 경우
	{
		if(selectedID != PlayerTileID) //플레이어가 서있는 곳을 클릭해야 선택됨
		{
			UE_LOG(LogTemp, Warning, TEXT("Not on Character%d : %d"), CurrentTurn, selectedID);
			return;
		}

		CurrentTileID = selectedID;
		TileMap[IDToIndex(CurrentTileID)]->ChangeTileSM(ETileType::Selected); //Cast<ATileCell>(aActor)->ChangeSMSelected();
		UE_LOG(LogTemp, Warning, TEXT("TileSelect: %d"), CurrentTileID);

		AvailableTileSM(CharacterTile[IDToIndex(PlayerTileID)]); //이동가능한 타일 표시 + 공격가능한 적의 타일도 표시
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TileRelease: %d -> %d"), CurrentTileID, selectedID);

		if (selectedID == PlayerTileID) //현재 플레이어가 서있는 곳을 클릭한 경우
		{
			CurrentTileID = -1;
			AttackTileID = -1;

			ClearTileSM();
			UE_LOG(LogTemp, Warning, TEXT("Cancle Move : %d"), selectedID);
			return;
		}

		int targetTileIndex = IDToIndex(selectedID);
		if (AttackTileID != -1) //적군이 클릭 된 상태
		{
			if (TileMap[targetTileIndex]->GetTileType() == ETileType::Attack) //공격 가능한 위치인 경우
			{
				AttackTile(CurrentTileID, selectedID, Player, CharacterTile[IDToIndex(AttackTileID)]);
			}
		}
		else
		{
			if (TileMap[targetTileIndex]->GetTileType() == ETileType::Available) //아무도 없으면 이동
			{
				MoveTile(CurrentTileID, selectedID, Player);
			}
			else if (CharacterTile[targetTileIndex]->GetIsPlayer()) //다른 플레이어가 서있는 곳을 클릭한 경우
			{
				UE_LOG(LogTemp, Warning, TEXT("%d Tile is Occupied"), selectedID);
				return;
			}
			else if (!CharacterTile[targetTileIndex]->GetIsPlayer()) //적군이 서있는 곳을 클릭한 경우
			{
				UE_LOG(LogTemp, Warning, TEXT("%d Enemy Attack"), selectedID);
				AttackTileSM(CharacterTile[IDToIndex(PlayerTileID)], selectedID);
				return;
			}
		}
	}
}

void ABattleState::ClearTileSM()
{
	for (auto tile : TileMap)
	{
		tile->ChangeTileSM(ETileType::Idle);
	}

	if (PlayerTileID != -1) //플레이어 턴이 안끝난 경우
	{
		if (AttackTileID != -1) //공격하려 할 때
		{
			TileMap[IDToIndex(PlayerTileID)]->ChangeTileSM(ETileType::Selected);
		}
		else //공격하지 않을 때
		{
			TileMap[IDToIndex(PlayerTileID)]->ChangeTileSM(ETileType::Current);
		}
	}
}
void ABattleState::AvailableTileSM(ABattleCharacter* CurrentPlayer)
{
	int StartTileID = CurrentPlayer->GetTileLocationID();
	int MoveRange = CurrentPlayer->GetMoveRange();
	int AttackRange = CurrentPlayer->GetAttackRange();

	int visited[100];
	for (int i = 0; i < 100; i++)
		visited[i] = 100;

	TQueue<int> q;
	q.Enqueue(StartTileID);
	visited[StartTileID] = 0;

	int v = -1; //ID임
	while (!q.IsEmpty())
	{
		q.Dequeue(v);

		if(CharacterTile[IDToIndex(v)] == nullptr)
		{
			TileMap[IDToIndex(v)]->ChangeTileSM(ETileType::Available);

			//공격 가능한 캐릭터 있는지 검사
			int t = v - AttackRange; //좌
			if (v % 10 >= AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if(CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
			t = v + AttackRange; //우
			if (v % 10 < BattleColumn - AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if (CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
			t = v - (10 * AttackRange); //상
			if (v / 10 >= AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if (CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
			t = v + (10 * AttackRange); //하
			if (v / 10 < BattleRow - AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if (CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
		}

		//이동 가능한 범위 체크
		int t = v - 1; //좌
		if (v % 10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if(visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
		t = v + 1; //우
		if (v % 10 < BattleColumn - 1 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if (visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
		t = v - 10; //상
		if (v / 10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if (visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
		t = v + 10; //하
		if (v / 10 < BattleRow - 1 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if (visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
	}
}
void ABattleState::AttackTileSM(ABattleCharacter* CurrentPlayer, int TargetID)
{
	AttackTileID = TargetID;
	TileMap[IDToIndex(AttackTileID)]->ChangeTileSM(ETileType::Selected);

	int AttackRange = CurrentPlayer->GetAttackRange();

	//타겟을 공격 가능할 수 있는 타일 검사
	int v = AttackTileID;

	int t = v - AttackRange; //좌
	if (v % 10 >= AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
	t = v + AttackRange; //우
	if (v % 10 < BattleColumn - AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
	t = v - (10 * AttackRange); //상
	if (v / 10 >= AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
	t = v + (10 * AttackRange); //하
	if (v / 10 < BattleRow - AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
}

int ABattleState::IDToIndex(int inTileID)
{
	return (inTileID/10)*BattleColumn + (inTileID%10);
}

void ABattleState::PlayTurn()
{
	bIsRunBehavior = true; //턴 종료 및 타일 클릭 비활성화
}

void ABattleState::NextTurn()
{
	if (Player.Num() == 0 || Enemy.Num() == 0)
	{
		//게임 종료
		UE_LOG(LogTemp, Error, TEXT("---------- Game End ----------"));
		UE_LOG(LogTemp, Error, TEXT("Turn Count: %d"), TurnCount);
		return;
	}

	if (bIsPlayerTurn && ++CurrentTurn >= Player.Num()) //플레이어의 턴 모두 종료
	{
		bIsPlayerTurn = false;
		CurrentTurn = 0;
	}
	else if(!bIsPlayerTurn && ++CurrentTurn >= Enemy.Num()) //적군의 턴 모두 종료
	{
		bIsPlayerTurn = true;
		CurrentTurn = 0;
	}
	
	if(bIsPlayerTurn)
	{
		PlayerTileID = Player[CurrentTurn]->GetTileLocationID();
		ClearTileSM(); //TileMap[IDToIndex(PlayerTileID)]->ChangeTileSM(ETileType::Current);
		TurnCount++;
	}
	else
	{
		TileMap[IDToIndex(Enemy[CurrentTurn]->GetTileLocationID())]->ChangeTileSM(ETileType::Current);
	}
		
	if(bIsPlayerTurn) //플레이어 턴이 된 경우 입력 가능하도록
	{
		bIsRunBehavior = false;
	}
	else //적군 턴 시작
	{
		//적군 AI 실행
		MoveTile(Enemy[CurrentTurn]->GetTileLocationID(), Enemy[CurrentTurn]->GetTileLocationID() - 1, Enemy); //왼쪽으로 한 칸 이동 (임시)
	}
}

//BFS로 길찾기
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

		int t = v-1; //좌
		if (v%10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v]+1;
			parent[t] = v;
			q.Enqueue(t);
		}
		t = v+1; //우
		if (v%10 < BattleColumn-1 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			parent[t] = v;
			q.Enqueue(t);
		}

		t = v-10; //상
		if (v/10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			parent[t] = v;
			q.Enqueue(t);
		}
		t = v+10; //하
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
		//UE_LOG(LogTemp, Warning, TEXT("Now V: %d"), v);
	}

	while (ReverseRoute.Num() != 0)
	{
		Route.Add(TileMap[IDToIndex(ReverseRoute.Last())]->GetActorLocation());
		ReverseRoute.RemoveAt(ReverseRoute.Num()-1); //마지막 원소 제거
	}

	//Route의 맨 첫번째는 시작위치가 저장되어 있어 지워도 될듯
	//for (auto vec : Route)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("(%f %f)"), vec.X, vec.Y);
	//}
}

void ABattleState::MoveTile(int StartTileID, int EndTileID, TArray<ABattleCharacter*>& BCharacter)
{
	TArray<FVector> ArrVec;
	FindRoute(BCharacter[CurrentTurn]->GetTileLocationID(), EndTileID, ArrVec);

	Cast<ABattleAIController>(BCharacter[CurrentTurn]->GetController())->MoveCharacter(ArrVec); //거쳐가야할 벡터 리스트 전달해야함

	CharacterTile[IDToIndex(EndTileID)] = CharacterTile[IDToIndex(StartTileID)]; //캐릭터 이동
	CharacterTile[IDToIndex(StartTileID)] = nullptr; //원래 자리는 공백으로
	BCharacter[CurrentTurn]->SetTileLocationID(EndTileID);

	PlayerTileID = -1; //이동 시엔 모든 타일 초기화
	CurrentTileID = -1;
	ClearTileSM();
}

void ABattleState::AttackTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter, ABattleCharacter* TargetCharacter)
{
	CharacterTile[IDToIndex(Player[CurrentTurn]->GetTileLocationID())]->SetTargetCharacter(TargetCharacter);

	//스테이트에 따라 공격할 건지 스킬을 쓸 건지 구분
	Cast<ABattleAIController>(BCharacter[CurrentTurn]->GetController())->AttackCharacter();
	//Cast<ABattleAIController>(BCharacter[CurrentTurn]->GetController())->SkillCharacter();

	AttackTileID = -1;

	MoveTile(StartTile, EndTile, BCharacter);
}

void ABattleState::DeleteCharacter(int TileLocID)
{
	ABattleCharacter* DeadCharacter = CharacterTile[IDToIndex(TileLocID)];
	if(DeadCharacter != nullptr)
	{
		if (DeadCharacter->GetIsPlayer())
		{
			UE_LOG(LogTemp, Warning, TEXT("Remove (%s)"), *Player[Player.Find(DeadCharacter)]->GetName());
			Player.Remove(DeadCharacter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Remove (%s)"), *Enemy[Enemy.Find(DeadCharacter)]->GetName());
			Enemy.Remove(DeadCharacter);
		}

		CharacterTile[IDToIndex(TileLocID)] = nullptr;
	}
}
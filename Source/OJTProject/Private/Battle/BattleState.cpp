// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleState.h"
#include "Battle/BattleAIController.h"
#include "Battle/BattlePlayerController.h"
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

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); //���� ���� �̸� �ҷ�����
	BattleLevel = FCString::Atoi(*(LevelName.Right(1))); //���� �̸��� ���� ������ ��ȣ ��������

	SpawnTiles(); //Ÿ�� ����
	SpawnCharacter(); //ĳ���� ����

	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnPlayTurnDelegate.AddDynamic(this, &ABattleState::PlayTurn);
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnNextTurnDelegate.AddDynamic(this, &ABattleState::NextTurn);


	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		const auto BattlePlayerController = Cast<ABattlePlayerController>(It->Get());
		if(BattlePlayerController != nullptr)
		{
			BPlayerController = BattlePlayerController;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("---------- Game Start ----------"));
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
	//UE_LOG(LogTemp, Warning, TEXT("Floor - X: %f, Y: %f"), floorLoc.X, floorLoc.Y);

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

	CharacterTile.Init(nullptr, BattleRow * BattleColumn);
}

void ABattleState::SpawnCharacter()
{
	CharacterSpawn(0, ECharacterType::PLAYER1);
	CharacterSpawn(10, ECharacterType::PLAYER2);
	CharacterSpawn(4, ECharacterType::ENEMY);
	CharacterSpawn(14, ECharacterType::ENEMY);
}

void ABattleState::ClickTile(AActor* aActor)
{
	if(bIsRunBehavior) return; //�ൿ���� ��� Ŭ�� �Ұ�

	int selectedID = Cast<ATileCell>(aActor)->GetTileID();

	if(bIsSpawn)
	{
		if(CharacterTile[IDToIndex(selectedID)] == nullptr)
		{
			CharacterSpawn(selectedID, SpawnType);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't spawn this tile : %d"), selectedID);
		}
		return;
	}

	if (TileMap[IDToIndex(selectedID)]->GetTileType() == ETileType::Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't click this tile : %d"), selectedID);
		return;
	}

	if(CurrentTileID == -1) //���õ� Ÿ���� ���� ���
	{
		if(selectedID != PlayerTileID) //�÷��̾ ���ִ� ���� Ŭ���ؾ� ���õ�
		{
			UE_LOG(LogTemp, Warning, TEXT("Not on Character%d : %d"), CurrentTurn, selectedID);
			return;
		}

		CurrentTileID = selectedID;
		TileMap[IDToIndex(CurrentTileID)]->ChangeTileSM(ETileType::Selected); //Cast<ATileCell>(aActor)->ChangeSMSelected();
		UE_LOG(LogTemp, Warning, TEXT("TileSelect: %d"), CurrentTileID);

		AvailableTileSM(CharacterTile[IDToIndex(PlayerTileID)]); //�̵������� Ÿ�� ǥ�� + ���ݰ����� ���� Ÿ�ϵ� ǥ��
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TileRelease: %d -> %d"), CurrentTileID, selectedID);

		if (selectedID == PlayerTileID && TileMap[IDToIndex(selectedID)]->GetTileType() != ETileType::Attack) //���� �÷��̾ ���ִ� ���� Ŭ���� ���
		{
			CurrentTileID = -1;
			AttackTileID = -1;

			ClearTileSM();
			UE_LOG(LogTemp, Warning, TEXT("Cancle Move : %d"), selectedID);
			return;
		}

		int targetTileIndex = IDToIndex(selectedID);
		if (AttackTileID != -1) //������ Ŭ�� �� ����
		{
			if (TileMap[targetTileIndex]->GetTileType() == ETileType::Attack) //���� ������ ��ġ�� ���
			{
				AttackTile(CurrentTileID, selectedID, Player, CharacterTile[IDToIndex(AttackTileID)]);
			}
		}
		else
		{
			if (TileMap[targetTileIndex]->GetTileType() == ETileType::Available) //�ƹ��� ������ �̵�
			{
				MoveTile(CurrentTileID, selectedID, Player);
			}
			else if (CharacterTile[targetTileIndex]->GetIsPlayer()) //�ٸ� �÷��̾ ���ִ� ���� Ŭ���� ���
			{
				UE_LOG(LogTemp, Warning, TEXT("%d Tile is Occupied"), selectedID);
				return;
			}
			else if (!CharacterTile[targetTileIndex]->GetIsPlayer()) //������ ���ִ� ���� Ŭ���� ���
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

	if (PlayerTileID != -1) //�÷��̾� ���� �ȳ��� ���
	{
		if (AttackTileID != -1) //�����Ϸ� �� ��
		{
			TileMap[IDToIndex(PlayerTileID)]->ChangeTileSM(ETileType::Selected);
		}
		else //�������� ���� ��
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
	TileMap[IDToIndex(StartTileID)]->ChangeTileSM(ETileType::Available); //�ڱ� �ڽ��� ��ġ�� ���ݰ����� ��ġ�� ����

	int v = -1; //ID��
	while (!q.IsEmpty())
	{
		q.Dequeue(v);

		if(CharacterTile[IDToIndex(v)] == nullptr || CharacterTile[IDToIndex(v)] == CurrentPlayer)
		{
			TileMap[IDToIndex(v)]->ChangeTileSM(ETileType::Available);

			//���� ������ ĳ���� �ִ��� �˻�
			int t = v - AttackRange; //��
			if (v % 10 >= AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if(CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
			t = v + AttackRange; //��
			if (v % 10 < BattleColumn - AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if (CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
			t = v - (10 * AttackRange); //��
			if (v / 10 >= AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if (CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
			t = v + (10 * AttackRange); //��
			if (v / 10 < BattleRow - AttackRange && CharacterTile[IDToIndex(t)] != nullptr)
			{
				if (CharacterTile[IDToIndex(t)]->GetIsPlayer() != CurrentPlayer->GetIsPlayer())
				{
					TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Enemy);
				}
			}
		}

		//�̵� ������ ���� üũ
		int t = v - 1; //��
		if (v % 10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if(visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
		t = v + 1; //��
		if (v % 10 < BattleColumn - 1 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if (visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
		t = v - 10; //��
		if (v / 10 > 0 && visited[t] > visited[v])
		{
			visited[t] = visited[v] + 1;
			if (visited[t] <= MoveRange)
			{
				q.Enqueue(t);
			}
		}
		t = v + 10; //��
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

	//Ÿ���� ���� ������ �� �ִ� Ÿ�� �˻�
	int v = AttackTileID;

	int t = v - AttackRange; //��
	if (v % 10 >= AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
	t = v + AttackRange; //��
	if (v % 10 < BattleColumn - AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
	t = v - (10 * AttackRange); //��
	if (v / 10 >= AttackRange && TileMap[IDToIndex(t)]->GetTileType() == ETileType::Available)
	{
		TileMap[IDToIndex(t)]->ChangeTileSM(ETileType::Attack);
	}
	t = v + (10 * AttackRange); //��
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
	bIsRunBehavior = true; //�� ���� �� Ÿ�� Ŭ�� ��Ȱ��ȭ
}

void ABattleState::NextTurn()
{
	if (Player.Num() == 0 || Enemy.Num() == 0)
	{
		//���� ����
		UE_LOG(LogTemp, Error, TEXT("---------- Game End ----------"));
		UE_LOG(LogTemp, Error, TEXT("Turn Count: %d"), TurnCount);

		if(Player.Num() == 0)
		{
			GameEnd(false);
		}
		else if(Enemy.Num() == 0)
		{
			GameEnd(true);
		}

		return;
	}

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
		PlayerTileID = Player[CurrentTurn]->GetTileLocationID();
		ClearTileSM(); //TileMap[IDToIndex(PlayerTileID)]->ChangeTileSM(ETileType::Current);
		TurnCount++;

		OnTurnCountDelegate.Broadcast(TurnCount); //�� ī��Ʈ�� ������ �ݿ�

		bIsRunBehavior = false; //�÷��̾� ���� �� ��� �Է� �����ϵ���
		//�ش� ĳ������ ��ų UI�� ȭ�鿡 ��ư���� �����ֱ�
	}
	else
	{
		PlayerTileID = Enemy[CurrentTurn]->GetTileLocationID();
		ClearTileSM();

		PlayEnemyTurn(); //���� AI ����
	}
}

void ABattleState::PlayEnemyTurn()
{
	CurrentTileID = PlayerTileID;

	AvailableTileSM(CharacterTile[IDToIndex(PlayerTileID)]); //�� ĳ������ ��ġ�� �������� �̵�/���� ������ Ÿ�� ã��

	TArray<int> enemyTargetTile;
	TArray<int> enemyMovableTile;
	for(int i = 0; i < BattleColumn * BattleRow; i++)
	{
		if(TileMap[i]->GetTileType() == ETileType::Enemy) //���� ��ġ�� Ÿ��
		{
			enemyTargetTile.Add(TileMap[i]->GetTileID());
		}
		else if(TileMap[i]->GetTileType() == ETileType::Available) //�̵� ������ Ÿ��
		{
			enemyMovableTile.Add(TileMap[i]->GetTileID());
		}
	}

	if(enemyTargetTile.Num() != 0) //������ ��밡 �ִ� ���
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy find target."));

		int randNum = FMath::RandRange(0, enemyTargetTile.Num()-1);
		AttackTileID = enemyTargetTile[randNum];
		AttackTileSM(Enemy[CurrentTurn], AttackTileID);

		TArray<int> enemyAttackTile;
		for(int i = 0; i < BattleColumn * BattleRow; i++)
		{
			if(TileMap[i]->GetTileType() == ETileType::Attack) //���� ������ Ÿ��
			{
				UE_LOG(LogTemp, Warning, TEXT("enemyAttackTile push %d"), TileMap[i]->GetTileID());
				enemyAttackTile.Add(TileMap[i]->GetTileID());
			}
		}

		if(enemyAttackTile.Num() != 0) //������ �� �ִ� ��ġ�� �ִ� ���
		{
			int randNum2 = FMath::RandRange(0, enemyAttackTile.Num() - 1);
			int attackPosID = enemyAttackTile[randNum2];
			UE_LOG(LogTemp, Warning, TEXT("Enemy attack %d on %d"), AttackTileID, attackPosID);

			int randSkill = FMath::RandRange(0, 9);
			if(randSkill < 3) //30% Ȯ���� ��ų ���
				ActiveSkill();
			
			AttackTile(CurrentTileID, attackPosID, Enemy, CharacterTile[IDToIndex(AttackTileID)]); //������ ��ġ���� ����
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy can't move."));
			AttackTileID = -1;
			MoveTile(CurrentTileID, CurrentTileID, Enemy); //�̵� ���ϱ� (�� �ѱ��)
		}
	}
	else if(enemyMovableTile.Num() != 0) //�̵��� �� �ִ� ���
	{
		int randNum2 = FMath::RandRange(0, enemyMovableTile.Num() - 1);
		int movePosID = enemyMovableTile[randNum2];
		MoveTile(CurrentTileID, movePosID, Enemy); //���� ��ġ�� �̵�

		UE_LOG(LogTemp, Warning, TEXT("Enemy move to %d"), movePosID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy can't move."));
		MoveTile(CurrentTileID, CurrentTileID, Enemy); //�̵� ���ϱ� (�� �ѱ��)
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
		//UE_LOG(LogTemp, Warning, TEXT("Now V: %d"), v);
	}

	while (ReverseRoute.Num() != 0)
	{
		Route.Add(TileMap[IDToIndex(ReverseRoute.Last())]->GetActorLocation());
		ReverseRoute.RemoveAt(ReverseRoute.Num()-1); //������ ���� ����
	}

	//Route�� �� ù��°�� ������ġ�� ����Ǿ� �־� ������ �ɵ�
	//for (auto vec : Route)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("(%f %f)"), vec.X, vec.Y);
	//}
}

void ABattleState::MoveTile(int StartTileID, int EndTileID, TArray<ABattleCharacter*>& BCharacter)
{
	TArray<FVector> ArrVec;
	FindRoute(BCharacter[CurrentTurn]->GetTileLocationID(), EndTileID, ArrVec);

	Cast<ABattleAIController>(BCharacter[CurrentTurn]->GetController())->MoveCharacter(ArrVec); //���İ����� ���� ����Ʈ �����ؾ���

	CharacterTile[IDToIndex(EndTileID)] = CharacterTile[IDToIndex(StartTileID)]; //ĳ���� �̵�
	if(StartTileID != EndTileID)
	{
		CharacterTile[IDToIndex(StartTileID)] = nullptr; //���� �ڸ��� ��������
	}
	BCharacter[CurrentTurn]->SetTileLocationID(EndTileID);

	PlayerTileID = -1; //�̵� �ÿ� ��� Ÿ�� �ʱ�ȭ
	CurrentTileID = -1;
	ClearTileSM();
}

void ABattleState::AttackTile(int StartTile, int EndTile, TArray<ABattleCharacter*>& BCharacter, ABattleCharacter* TargetCharacter)
{
	CharacterTile[IDToIndex(BCharacter[CurrentTurn]->GetTileLocationID())]->SetTargetCharacter(TargetCharacter);

	//������Ʈ�� ���� ������ ���� ��ų�� �� ���� ����
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

bool ABattleState::ActiveSkill()
{
	ABattleCharacter* TurnCharacter;
	if (bIsPlayerTurn)
	{
		TurnCharacter = Player[CurrentTurn];
	}
	else
	{
		TurnCharacter = Enemy[CurrentTurn];
	}

	//���� ĳ������ ���¸� ��ų�� ����
	if (TurnCharacter->IsSkillState()) //�̹� ��ų�� ����ϴ� ���¶��
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Deactivate"));
		TurnCharacter->SetAttackState(); //�ٽ� ���� ���·� ��������
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Activate"));
		TurnCharacter->SetSkillState();
		return true;
	}
}

bool ABattleState::GetIsRunBehavior()
{
	return bIsRunBehavior;
}

void ABattleState::GameEnd(bool bIsWin)
{
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnPlayTurnDelegate.RemoveAll(this);
	Cast<UMapGameInstance>(GetGameInstance())->GetTurnManagerInstance()->OnNextTurnDelegate.RemoveAll(this);

	if (BPlayerController != nullptr)
	{
		BPlayerController->ShowResultUI(bIsWin, TurnCount); //��� â �����ֱ�
	}

	//3�� �� �κ�� �̵�
	FTimerHandle EndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, FTimerDelegate::CreateLambda([this]() {

		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
		
	}), 3.0f, false);
}

void ABattleState::ReadyCharacterSpawn(ECharacterType CType)
{
	if(bIsSpawn && CType == SpawnType) //�Ȱ��� ��ư�� �ٽ� ���� ���
	{
		if (BPlayerController != nullptr)
		{
			BPlayerController->ClearSpawnButtonColor();
		}
		bIsSpawn = false;
		SpawnType = ECharacterType::NONE;
	}
	else
	{
		bIsSpawn = true;
		SpawnType = CType;
	}
}

void ABattleState::CharacterSpawn(int SpawnID, ECharacterType CType)
{
	int SpawnIndex = IDToIndex(SpawnID);
	ABattleCharacter* BTChar;
	if(CType == ECharacterType::PLAYER1)
	{
		BTChar = GetWorld()->SpawnActor<ACharacterShort>(TileMap[SpawnIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player" + FString::FromInt(Player.Num()));
		Player.Add(BTChar);
	}
	else if(CType == ECharacterType::PLAYER2)
	{
		BTChar = GetWorld()->SpawnActor<ACharacterLong>(TileMap[SpawnIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
		BTChar->SetActorLabel("Player" + FString::FromInt(Player.Num()));
		Player.Add(BTChar);
	}
	else if(CType == ECharacterType::ENEMY)
	{
		BTChar = GetWorld()->SpawnActor<ACharacterEnemy>(TileMap[SpawnIndex]->GetTransform().GetLocation() + FVector(0, 0, 100), FRotator(0, 180, 0));
		BTChar->SetActorLabel("Enemy" + FString::FromInt(Enemy.Num()));
		Enemy.Add(BTChar);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn NONE"));
		return;
	}

	BTChar->SetTileLocationID(TileMap[SpawnIndex]->GetTileID());
	BTChar->OnNotifyDeadDelegate.AddDynamic(this, &ABattleState::DeleteCharacter);
	CharacterTile[SpawnIndex] = BTChar;

	bIsSpawn = false;
	SpawnType = ECharacterType::NONE;
	if(BPlayerController != nullptr)
	{
		BPlayerController->ClearSpawnButtonColor();
	}
}
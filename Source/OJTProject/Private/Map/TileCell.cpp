// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileCell.h"

// Sets default values
ATileCell::ATileCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Cell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell"));
	Cell->SetRelativeScale3D(FVector(1.9f, 1.9f, 1.0f));
	
	RootComponent = Cell;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLANE(TEXT("StaticMesh'/Game/StaticMesh/Shape_Plane.Shape_Plane'"));
	if (SM_PLANE.Succeeded())
	{
		SMIdle = SM_PLANE.Object;
		ChangeTileSM(ETileType::Idle);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CURRENT(TEXT("StaticMesh'/Game/StaticMesh/Plane_Current.Plane_Current'"));
	if (SM_CURRENT.Succeeded())
		SMCurrent = SM_CURRENT.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SELECTED(TEXT("StaticMesh'/Game/StaticMesh/Plane_Selected.Plane_Selected'"));
	if (SM_SELECTED.Succeeded())
		SMSelected = SM_SELECTED.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_AVAILABLE(TEXT("StaticMesh'/Game/StaticMesh/Plane_Available.Plane_Available'"));
	if (SM_AVAILABLE.Succeeded())
		SMAvailable = SM_AVAILABLE.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ENEMY(TEXT("StaticMesh'/Game/StaticMesh/Plane_Enemy.Plane_Enemy'"));
	if (SM_ENEMY.Succeeded())
		SMEnemy = SM_ENEMY.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ATTACK(TEXT("StaticMesh'/Game/StaticMesh/Plane_Attack.Plane_Attack'"));
	if (SM_ATTACK.Succeeded())
		SMAttack = SM_ATTACK.Object;

	
	OnClicked.AddDynamic(this, &ATileCell::ClickTile);
}

void ATileCell::ClickTile(AActor* TileActor, FKey TileKey)
{
	OnTileSelectedDelegate.Broadcast(TileActor);
}

void ATileCell::BeginPlay()
{
	Super::BeginPlay();
}

void ATileCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileCell::SetTileID(int ID)
{
	TileID = ID;
}

int ATileCell::GetTileID()
{
	return TileID;
}

void ATileCell::ChangeTileSM(ETileType inType)
{
	TileType = inType;

	if(TileType == ETileType::Idle)
		Cell->SetStaticMesh(SMIdle);
	else if(TileType == ETileType::Current)
		Cell->SetStaticMesh(SMCurrent);
	else if (TileType == ETileType::Selected)
		Cell->SetStaticMesh(SMSelected);
	else if (TileType == ETileType::Available)
		Cell->SetStaticMesh(SMAvailable);
	else if (TileType == ETileType::Enemy)
		Cell->SetStaticMesh(SMEnemy);
	else if (TileType == ETileType::Attack)
		Cell->SetStaticMesh(SMAttack);
}
ETileType ATileCell::GetTileType()
{
	return TileType;
}
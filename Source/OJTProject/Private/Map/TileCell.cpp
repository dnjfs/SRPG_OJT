// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileCell.h"

// Sets default values
ATileCell::ATileCell() : TileID(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Cell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell"));
	Cell->SetRelativeScale3D(FVector(1.9f, 1.9f, 1.0f));
	
	RootComponent = Cell;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLANE(TEXT("StaticMesh'/Game/StaticMesh/Shape_Plane.Shape_Plane'"));
	if (SM_PLANE.Succeeded())
	{
		Idle = SM_PLANE.Object;
		Cell->SetStaticMesh(Idle);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SELECTED(TEXT("StaticMesh'/Game/StaticMesh/Plane_Selected.Plane_Selected'"));
	if (SM_PLANE.Succeeded())
		Selected = SM_SELECTED.Object;

	
	//OnBeginCursorOver.AddDynamic(this, &ATileCell::PrintName);
	OnClicked.AddDynamic(this, &ATileCell::ClickTile);
	//OnReleased.AddDynamic(this, &ATileCell::PrintName3);

	//UE_LOG(LogTemp, Warning, TEXT("Complete Construct"));
	//OnClicked.Broadcast(this, FKey(""));
}

void ATileCell::PrintName(AActor* TileActor)
{
	UE_LOG(LogTemp, Warning, TEXT("TileOver %d"), TileID);
}
void ATileCell::ClickTile(AActor* TileActor, FKey TileKey)
{
	OnTileSelecedDelegate.Broadcast(TileActor);
	//Cell->SetStaticMesh(Selected);
	//UE_LOG(LogTemp, Warning, TEXT("TileClick %d"), TileID);
}
void ATileCell::PrintName3(AActor* TileActor, FKey TileKey)
{
	Cell->SetStaticMesh(Idle);
	UE_LOG(LogTemp, Warning, TEXT("TileRelease %d"), TileID);
}

// Called when the game starts or when spawned
void ATileCell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
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

void ATileCell::ChangeSMIdle()
{
	Cell->SetStaticMesh(Idle);
}
void ATileCell::ChangeSMSelected()
{
	Cell->SetStaticMesh(Selected);
}
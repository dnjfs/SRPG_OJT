// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileCell.h"

// Sets default values
ATileCell::ATileCell() : TileID(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell"));
	
	RootComponent = Cell;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLANE(TEXT("StaticMesh'/Game/StaticMesh/Shape_Plane.Shape_Plane'"));
	if (SM_PLANE.Succeeded())
	{
		Cell->SetStaticMesh(SM_PLANE.Object);
	}

	Cell->SetRelativeScale3D(FVector(1.9f, 1.9f, 1.0f));

	OnBeginCursorOver.AddDynamic(this, &ATileCell::PrintName);
	OnClicked.AddDynamic(this, &ATileCell::PrintName2);
}

void ATileCell::PrintName(AActor* TileActor)
{
	UE_LOG(LogTemp, Warning, TEXT("TileOver %d"), TileID);
}
void ATileCell::PrintName2(AActor* TileActor, FKey TileKey)
{
	UE_LOG(LogTemp, Warning, TEXT("TileClick %d"), TileID);
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
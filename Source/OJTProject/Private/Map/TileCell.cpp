// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TileCell.h"

// Sets default values
ATileCell::ATileCell()
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

	Cell->SetRelativeScale3D(FVector(2.0f, 2.0f, 1.0f));
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


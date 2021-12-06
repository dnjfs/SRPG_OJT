// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "TileCell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileSelectedDelegate, AActor*, TouchedActor);

enum class ETileType
{
	Idle,
	Current,
	Selected,
	Available,
	Enemy,
};

UCLASS()
class OJTPROJECT_API ATileCell : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATileCell();
	virtual ~ATileCell() { /*UE_LOG(LogTemp, Warning, TEXT("Deleted Tile%d"), TileID);*/ }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTileID(int ID);
	int GetTileID();

	UFUNCTION()
	void ClickTile(AActor* TileActor, FKey TileKey);

	void ChangeTileSM(ETileType inType);

	FOnTileSelectedDelegate OnTileSelectedDelegate;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Cell;

	UPROPERTY()
	UStaticMesh* SMIdle;
	UPROPERTY()
	UStaticMesh* SMCurrent;
	UPROPERTY()
	UStaticMesh* SMSelected;
	UPROPERTY()
	UStaticMesh* SMAvailable;
	UPROPERTY()
	UStaticMesh* SMEnemy;

	UPROPERTY()
	int32 TileID = -1; //За*10 + ї­
};

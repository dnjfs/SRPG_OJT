// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileCell.generated.h"

UCLASS()
class OJTPROJECT_API ATileCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileCell();
	virtual ~ATileCell() { UE_LOG(LogTemp, Warning, TEXT("Deleted Tile%d"), TileID); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTileID(int ID);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Cell;

	UPROPERTY()
	int32 TileID;
};

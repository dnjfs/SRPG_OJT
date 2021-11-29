// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BattleCharacter.generated.h"

UCLASS()
class OJTPROJECT_API ABattleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABattleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetTileLocationID(int onLocID);
	int32 GetTileLocationID();

	void SetPlayerCharacter(bool inIsPlayer);
	bool GetIsPlayer();

private:
	bool bIsPlayer = false; //플레이어 캐릭터 여부
	int32 TileLocID = -1; //타일 ID
	int32 HP = 0;
	int32 Power = 0;
};

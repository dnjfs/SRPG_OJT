// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Map/MapGameInstance.h"
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
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void AttackCharacter();

	void SetTileLocationID(int onLocID);
	int32 GetTileLocationID();

	void SetPlayerCharacter(bool inIsPlayer);
	bool GetIsPlayer();

	void SetTargetCharacter(ABattleCharacter* inTarget);
	void ChangeHP(float Damage);

private:
	ECharacterType CharacterType = ECharacterType::NONE;
	bool bIsPlayer = false; //�÷��̾� ĳ���� ����
	int32 TileLocID = -1; //Ÿ�� ID

	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	int32 HP = 0;
	int32 Power = 0;

	ABattleCharacter* TargetCharacter;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"

#include "Map/MapGameInstance.h"
#include "Battle/PlayerAnimInstance.h"
#include "BattleCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyDeadDelegate, int, TileLocID);

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

	void SetCharacterType(ECharacterType inType);

	void PlayAttackAnimation();
	void PlaySkillAnimation();
	void AttackCharacter();
	void SkillCharacter(); //스킬을 쓸 땐 노티파이에서 이걸 불러오면 될지도.. 데미지도 다르게 할 수 있고 사운드, 이펙트도 다르게 할 수 있을 듯 함
	//혹은 함수 새로 안만들고 Enum으로 스테이트를 다르게 하여 나누면 될듯??

	void SetTileLocationID(int onLocID);
	int32 GetTileLocationID();

	void SetPlayerCharacter(bool inIsPlayer);
	bool GetIsPlayer();

	int32 GetMoveRange() { return MoveRange; }
	int32 GetAttackRange() { return AttackRange; }

	void SetTargetCharacter(ABattleCharacter* inTarget);
	void ChangeHP(float Damage);
	void DeadCharacter();

	FOnNotifyDeadDelegate OnNotifyDeadDelegate;

protected:
	ECharacterType CharacterType = ECharacterType::NONE;
	bool bIsPlayer = false; //플레이어 캐릭터 여부

	int32 MaxHP = 0;
	UPROPERTY(Transient, VisibleInstanceOnly) //Transient로 직렬화에서 제외
	int32 CurrentHP = 0;
	int32 Power = 0;
	int32 MoveRange = 0;
	int32 AttackRange = 0;
	float Coefficient = 0; //스킬에서 공격력 계수

private:
	int32 TileLocID = -1; //타일 ID

	UPlayerAnimInstance* PlayerAnim;
	ABattleCharacter* TargetCharacter;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;
	class UProgressBar* HPBar; //혹은 캐릭터 위젯 클래스를 만들어서 HP 바뀌면 델리게이트로 이벤트 전달하기?
};

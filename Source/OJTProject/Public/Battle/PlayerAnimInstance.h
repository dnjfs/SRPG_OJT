// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndAttackDelegate);

/**
 * 
 */
UCLASS()
class OJTPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetAttackMontage(UAnimMontage* inAttackMontage);
	void SetSkillMontage(UAnimMontage* inSkillMontage);
	void SetHitMontage(UAnimMontage* inHitMontage);

	UFUNCTION(BlueprintCallable)
	void AnimNotify_AttackHit();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_MontageEnded();

	void PlayAttackMontage();
	void PlaySkillMontage();
	void PlayHitMontage();

	void SetIsDead();

public:
	FOnAttackHitDelegate OnAttackHit; //ĳ���Ϳ��� ���
	FOnEndAttackDelegate OnEndAttack; //AI��Ʈ�ѷ����� ���
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;
};

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

	//void SetAttackMontage(UAnimMontage* inAttackMontage);
	//void SetSkillMontage(UAnimMontage* inSkillMontage);
	//void SetHitMontage(UAnimMontage* inHitMontage);

	UFUNCTION(BlueprintCallable)
	void AnimNotify_AttackHit();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_MontageEnded();

	void PlayAttackMontage();
	void PlaySkillMontage();
	void PlayHitMontage();

	void SetIsDead();

public:
	FOnAttackHitDelegate OnAttackHit; //캐릭터에서 사용
	FOnEndAttackDelegate OnEndAttack; //AI컨트롤러에서 사용
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;
};

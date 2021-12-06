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

	UFUNCTION(BlueprintCallable)
	void AnimNotify_AttackHit();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_MontageEnded();

	void PlayAttackMontage();

public:
	FOnAttackHitDelegate OnAttackHit; //캐릭터에서 사용
	FOnEndAttackDelegate OnEndAttack; //AI컨트롤러에서 사용
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};

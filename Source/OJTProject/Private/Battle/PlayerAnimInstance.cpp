// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	bIsDead = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void UPlayerAnimInstance::SetAttackMontage(UAnimMontage* inAttackMontage)
{
	AttackMontage = inAttackMontage;
}
void UPlayerAnimInstance::SetSkillMontage(UAnimMontage* inSkillMontage)
{
	SkillMontage = inSkillMontage;
}
void UPlayerAnimInstance::SetHitMontage(UAnimMontage* inHitMontage)
{
	HitMontage = inHitMontage;
}

void UPlayerAnimInstance::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_MontageEnded()
{
	OnEndAttack.Broadcast();
}

void UPlayerAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}
void UPlayerAnimInstance::PlaySkillMontage()
{
	Montage_Play(SkillMontage, 1.0f);
}
void UPlayerAnimInstance::PlayHitMontage()
{
	Montage_Play(HitMontage, 1.0f);
}

void UPlayerAnimInstance::SetIsDead()
{
	bIsDead = true;
}
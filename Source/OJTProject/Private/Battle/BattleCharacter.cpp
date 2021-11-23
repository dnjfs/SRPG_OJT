// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleCharacter.h"

// Sets default values
ABattleCharacter::ABattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//SkeletalMesh 가져오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_MANNEQUIN.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("SK Success"));
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	//AnimInstance 가져오기
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> MANNEQUIN_ANIM(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));
	if (MANNEQUIN_ANIM.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("ANIM Success"));
		GetMesh()->SetAnimInstanceClass(MANNEQUIN_ANIM.Class);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ANIM Failed"));

}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABattleCharacter::SetLocation(int onLoc)
{
	loc = onLoc;
}

int32 ABattleCharacter::GetLocation()
{
	return loc;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterShort.h"

ACharacterShort::ACharacterShort()
{
	CharacterType = ECharacterType::PLAYER1;
	bIsPlayer = true;
	HP = 100;
	Power = 40;
	MoveRange = 3;
	AttackRange = 1;
}
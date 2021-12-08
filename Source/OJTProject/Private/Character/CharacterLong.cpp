// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterLong.h"

ACharacterLong::ACharacterLong()
{
	CharacterType = ECharacterType::PLAYER2;
	bIsPlayer = true;
	MaxHP = 100;
	Power = 60;
	MoveRange = 3;
	AttackRange = 2;
}
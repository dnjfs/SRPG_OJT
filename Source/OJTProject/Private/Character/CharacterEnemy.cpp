// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterEnemy.h"

ACharacterEnemy::ACharacterEnemy()
{
	CharacterType = ECharacterType::ENEMY;
	bIsPlayer = false;
	MaxHP = 100;
	Power = 40;
	MoveRange = 3;
	AttackRange = 1;
	Coefficient = 0.75f;
}
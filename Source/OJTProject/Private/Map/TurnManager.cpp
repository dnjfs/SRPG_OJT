// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/TurnManager.h"


void UTurnManager::PlayTurnDelegate()
{
	OnPlayTurnDelegate.Broadcast();
}
void UTurnManager::NextTurnDelegate()
{
	OnNextTurnDelegate.Broadcast();
}
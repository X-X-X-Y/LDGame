// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LDTopDownPlayerState.h"


ALDTopDownPlayerState::ALDTopDownPlayerState()
{
	bHasSelectHero = false;
}

bool ALDTopDownPlayerState::CheckHasSelectHero() const
{
	return bHasSelectHero;
}

void ALDTopDownPlayerState::SetHasSelectHero(bool NewBoolValue)
{
	bHasSelectHero = NewBoolValue;
}

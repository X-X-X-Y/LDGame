// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LDTopDownPlayerState.h"

#include "Character/Hero/LDPlayerPawn.h"


ALDTopDownPlayerState::ALDTopDownPlayerState()
{
	
}

EPlayerSelectState ALDTopDownPlayerState::GetPlayerSelectState() const
{
	return PlayerSelectState;
}

void ALDTopDownPlayerState::SetPlayerSelectState(EPlayerSelectState NewState)
{
	PlayerSelectState = NewState;
}



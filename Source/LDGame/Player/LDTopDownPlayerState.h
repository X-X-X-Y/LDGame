// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/LDPlayerState.h"
#include "LDTopDownPlayerState.generated.h"


UENUM(BlueprintType)
enum class EPlayerSelectState : uint8
{
	OnSelectHero,
	OnSelectNone
};

UCLASS()
class LDGAME_API ALDTopDownPlayerState : public ALDPlayerState
{
	GENERATED_BODY()

public:
	ALDTopDownPlayerState();

	EPlayerSelectState GetPlayerSelectState() const;
	void SetPlayerSelectState(EPlayerSelectState NewState);
	
private:
	EPlayerSelectState PlayerSelectState = EPlayerSelectState::OnSelectNone;
};

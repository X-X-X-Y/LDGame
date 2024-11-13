// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/LDGameModeBase.h"
#include "LDTopGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LDGAME_API ALDTopGameMode : public ALDGameModeBase
{
	GENERATED_BODY()
	
public:
protected:
	virtual void BeginPlay() override;
};

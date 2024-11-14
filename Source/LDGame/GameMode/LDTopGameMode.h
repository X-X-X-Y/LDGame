// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/LDGameModeBase.h"
#include "LDTopGameMode.generated.h"

struct FDataRegistryAcquireResult;
class ALDHeroCharacter;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDTopGameMode : public ALDGameModeBase
{
	GENERATED_BODY()
	
public:

public:
	TArray<ALDHeroCharacter*> CurrentHeroPawnList;
	
protected:
	virtual void BeginPlay() override;

private:
	void InitHeroData();
	void SpawnHeroActors(const FDataRegistryAcquireResult& VehListHandle);
};

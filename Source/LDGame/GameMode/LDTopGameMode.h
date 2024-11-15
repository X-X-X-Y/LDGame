// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/LDGameModeBase.h"
#include "LDTopGameMode.generated.h"

struct FDataRegistryAcquireResult;
class ALDCharacterHero;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDTopGameMode : public ALDGameModeBase
{
	GENERATED_BODY()

public:
	
	ALDTopGameMode();

	void AddNewHeroInList(ALDCharacterHero* HeroActor); 
	TArray<ALDCharacterHero*> GetCurrentHeroActors();
	ALDCharacterHero* GetSelectHeroActor();

protected:
	virtual void BeginPlay() override;

private:
	void InitHeroData();
	void SpawnHeroActors(const FDataRegistryAcquireResult& VehListHandle);

private:
	TArray<ALDCharacterHero*> CurrentHeroActorList;
	int CurrentHeroActorIndex = 0;
};

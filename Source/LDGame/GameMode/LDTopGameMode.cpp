// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LDTopGameMode.h"

#include "DataRegistrySubsystem.h"
#include "EngineUtils.h"
#include "GameData/DataTable/LDHeroDataConfig.h"
#include "GameDevUtil/LDGameCommonFName.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameDevUtil/LDLogChannels.h"
#include "LevelActor/LDCharacterSpawnActor.h"

ALDTopGameMode::ALDTopGameMode()
{
	CurrentHeroActorList.Empty();
}

void ALDTopGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitHeroData();
}

#pragma region XiuShi Actor

void ALDTopGameMode::InitHeroData()
{
	UDataRegistrySubsystem* DRSubsystem = UDataRegistrySubsystem::Get();
	if (DRSubsystem)
	{
		//TODO:需要从LevelSubsystem中获取关卡信息,先暂直接测试修士
		FName RegistryID = LDGameCommonFName::NAME_XiuShiHeroRegistryID;

		FDataRegistryId HeroDataID(RegistryID, LDGameplayTags::XiuShiHeroTag_TestXiuShi.GetTag().GetTagName());
		DRSubsystem->AcquireItem(
			HeroDataID, FDataRegistryItemAcquiredCallback::CreateUObject(this, &ThisClass::SpawnHeroActors));
	}
}

void ALDTopGameMode::SpawnHeroActors(const FDataRegistryAcquireResult& VehListHandle)
{
	if (const FLDXiuShiHeroDataConfig* HeroData = VehListHandle.GetItem<FLDXiuShiHeroDataConfig>())
	{
		//TODO:到时候思考一下英雄出生点的方式
		for (TActorIterator<ALDCharacterSpawnActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ALDCharacterSpawnActor* SpawnPoint = Cast<ALDCharacterSpawnActor>(*ActorItr);
			if (SpawnPoint && SpawnPoint->SpawnCharacterType == ESpawnCharacterType::HeroActor)
			{
				SpawnPoint->SpawnHeroCharacterActor(HeroData->XiuShiHeroPawn);
			}
		}
	}
}

void ALDTopGameMode::AddNewHeroInList(ALDCharacterHero* HeroActor)
{
	if (HeroActor)
	{
		CurrentHeroActorList.AddUnique(HeroActor);
	}
}

TArray<ALDCharacterHero*> ALDTopGameMode::GetCurrentHeroActors()
{
	return CurrentHeroActorList;
}

ALDCharacterHero* ALDTopGameMode::GetSelectHeroActor()
{
	return CurrentHeroActorList[CurrentHeroActorIndex];
}


#pragma endregion

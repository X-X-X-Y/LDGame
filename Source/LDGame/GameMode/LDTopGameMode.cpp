// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LDTopGameMode.h"

#include "DataRegistrySubsystem.h"
#include "EngineUtils.h"
#include "GameData/DataTable/LDHeroDataConfig.h"
#include "GameDevUtil/LDGameCommonFName.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameDevUtil/LDLogChannels.h"
#include "LevelActor/LDCharacterSpawnActor.h"

void ALDTopGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitHeroData();
	// UDataRegistrySubsystem* DRSubsystem = UDataRegistrySubsystem::Get();
	// if (DRSubsystem)
	// {
	// 	TArray<FDataRegistrySourceItemId> HeroDataConfigs;
	// 	UDataRegistry* Test = DRSubsystem->GetRegistryForType(LDGameDataRegistryType::NAME_XiuShiHeroRegistryID);
	// 	if (Test)
	// 	{
	// 		Test->GetAllSourceItems(HeroDataConfigs);
	// 		if (HeroDataConfigs.Num() > 0)
	// 		{
	// 			UE_LOG(LogLD, Log, TEXT("Got it!"))
	// 		}
	// 	}
	// }
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


#pragma endregion

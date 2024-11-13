// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LDTopGameMode.h"

#include "DataRegistrySubsystem.h"
#include "GameData/DataTable/LDHeroDataConfig.h"
#include "GameDevUtil/LDGameCommonFName.h"
#include "GameDevUtil/LDLogChannels.h"

void ALDTopGameMode::BeginPlay()
{
	Super::BeginPlay();


	//TODO:需要从LevelSubsystem中获取关卡信息,先暂直接测试修士
	
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

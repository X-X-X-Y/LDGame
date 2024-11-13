// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LDGameLevelSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LDGAME_API ULDGameLevelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//TODO:这里订阅主菜单中关卡信息(XiuShiID、EquipID、EnemyID等等)
};

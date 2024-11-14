// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "Character/Hero/LDHeroCharacter.h"
#include "LDHeroDataConfig.generated.h"

class ALDCharacterHero;

USTRUCT(BlueprintType)
struct FLDXiuShiHeroDataConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FText XiuShiName;

	//-------------------------------------Attribute-------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="XiuShiAttribute")
	float XiuShiBaseHealth = 0;

	//-------------------------------------Pawn-------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="XiuShiPawn")
	TSoftClassPtr<ALDCharacterHero> XiuShiHeroPawn;
};

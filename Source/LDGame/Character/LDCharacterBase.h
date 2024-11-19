// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LDCharacterBase.generated.h"

class ULDAbilitySystemComponent;
class ULDAbilitySet;

UCLASS()
class LDGAME_API ALDCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALDCharacterBase();
};

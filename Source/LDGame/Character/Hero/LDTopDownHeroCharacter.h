// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Hero/LDHeroCharacter.h"
#include "LDTopDownHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LDGAME_API ALDTopDownHeroCharacter : public ALDHeroCharacter
{
	GENERATED_BODY()

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

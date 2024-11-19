// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/LDPlayerController.h"
#include "LDTopDownPlayerController.generated.h"



enum class EPlayerSelectState : uint8;
class ULDInputConfig;
struct FInputActionValue;
class ALDPlayerPawn;


UCLASS()
class LDGAME_API ALDTopDownPlayerController : public ALDPlayerController
{
	GENERATED_BODY()

public:
	ALDTopDownPlayerController();


};

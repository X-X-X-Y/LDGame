// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LDPlayerController.generated.h"

class ALDPlayerState;
class ULDAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "LDGame|PlayerController")
	ALDPlayerState* GetLDPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "InputSystem|ISAbilitySystemComponent")
	ULDAbilitySystemComponent* GetISAbilitySystemComponent() const;

protected:
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};

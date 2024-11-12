// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Hero/LDPlayerPawn.h"
#include "LDHeroCharacter.generated.h"

class ALDPlayerController;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDHeroCharacter : public ALDCharacterBase
{
	GENERATED_BODY()

public:
	ALDHeroCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnInputStarted(const FInputActionValue& Value);
	void OnSetDestinationTriggered(const FInputActionValue& Value);
	void OnSetDestinationReleased(const FInputActionValue& Value);

public:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LDGame|Input")
	float ShortPressThreshold;
	
private:
	FVector CachedDestination;
	float FollowTime;
};

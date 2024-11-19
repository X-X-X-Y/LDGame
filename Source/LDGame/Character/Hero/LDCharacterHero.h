// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LDCharacterBase.h"
#include "LDCharacterHero.generated.h"

struct FInputActionValue;
struct FGameplayTag;
class ULDInputConfig;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDCharacterHero : public ALDCharacterBase
{
	GENERATED_BODY()
public:

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	void OnInputStarted(const FInputActionValue& InputValue);
	void OnSetDestinationTriggered(const FInputActionValue& Value);
	void OnSetDestinationReleased(const FInputActionValue& Value);
	void OnHeroCancelSelect(const FInputActionValue& Value);

public:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LDGame|Input")
	float ShortPressThreshold;

private:
	FVector CachedDestination;
	float FollowTime;
};

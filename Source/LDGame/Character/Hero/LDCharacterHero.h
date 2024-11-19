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
	
	ULDAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void OnInputStarted(const FInputActionValue& InputValue);
	void OnSetDestinationTriggered(const FInputActionValue& Value);
	void OnSetDestinationReleased(const FInputActionValue& Value);
	void OnHeroCancelSelect(const FInputActionValue& Value);

public:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LDGame|Input")
	float ShortPressThreshold;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* HeroInputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* HeroMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Abilities")
	ULDAbilitySet* AbilitySet;
	
	TWeakObjectPtr<class ULDAbilitySystemComponent> AbilitySystemComponent;

private:
	FVector CachedDestination;
	float FollowTime;
};

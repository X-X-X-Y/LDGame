// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LDGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ELDAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,
	
	// Continually try to activate the ability while the input is active.
	WhileInputActive
};

UCLASS()
class LDGAME_API ULDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ELDAbilityActivationPolicy GetActivationPolicy() const {return ActivationPolicy;}
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Ability Activation")
	ELDAbilityActivationPolicy ActivationPolicy;
};

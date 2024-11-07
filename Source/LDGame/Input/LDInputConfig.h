// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LDInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FLDInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class LDGAME_API ULDInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	
	// Returns the first Input Action associated with a given tag.
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	// Returns the first Input Action associated with a given tag.
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

public:
	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLDInputAction> NativeInputActions;

	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLDInputAction> AbilityInputActions;
};

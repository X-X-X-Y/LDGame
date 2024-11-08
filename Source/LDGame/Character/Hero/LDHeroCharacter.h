// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LDGame/Character/LDCharacterBase.h"
#include "LDHeroCharacter.generated.h"

struct FGameplayTag;
class ULDInputConfig;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDHeroCharacter : public ALDCharacterBase
{
	GENERATED_BODY()
	
public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* InputConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* DefaultMappingContext;
};

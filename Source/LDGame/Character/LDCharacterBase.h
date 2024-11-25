// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LDCharacterBase.generated.h"

class USphereComponent;
struct FGameplayTag;
class ULDInputConfig;
class ULDAbilitySystemComponent;
class ULDAbilitySet;

UCLASS()
class LDGAME_API ALDCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALDCharacterBase();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	virtual void BeginPlay() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* InputConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Abilities")
	ULDAbilitySet* AbilitySet;
	
	TWeakObjectPtr<class ULDAbilitySystemComponent> AbilitySystemComponent;
};

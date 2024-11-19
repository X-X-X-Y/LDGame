// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "LDPlayerState.generated.h"

class ULDAbilitySystemComponent;

UENUM(BlueprintType)
enum class EPlayerSelectState : uint8
{
	OnSelectHero,
	OnSelectNone
};

UCLASS()
class LDGAME_API ALDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ALDPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	EPlayerSelectState GetPlayerSelectState() const;
	void SetPlayerSelectState(EPlayerSelectState NewState);
	
private:
	EPlayerSelectState PlayerSelectState = EPlayerSelectState::OnSelectNone;
protected:
	UPROPERTY()
	ULDAbilitySystemComponent* AbilitySystemComponent;
};

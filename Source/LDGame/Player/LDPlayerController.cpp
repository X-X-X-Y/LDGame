// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"

#include "LDPlayerState.h"
#include "Character/Abilities/LDAbilitySystemComponent.h"

ALDPlayerState* ALDPlayerController::GetLDPlayerState() const
{
	return CastChecked<ALDPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

#pragma region PC-AbilitySystem

ULDAbilitySystemComponent* ALDPlayerController::GetISAbilitySystemComponent() const
{
	const ALDPlayerState* PS = GetLDPlayerState();
	return CastChecked<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
}

void ALDPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ALDPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(ULDAbilitySystemComponent* ASC = GetISAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

#pragma endregion



// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"

#include "LDPlayerState.h"
#include "Character/AbilitySystem/LDAbilitySystemComponent.h"
#include "Character/Hero/LDCharacterPlayer.h"
#include "Input/LDInputComponent.h"

ALDPlayerController::ALDPlayerController()
{
}

#pragma region APControl-Intreface

void ALDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerPawn = Cast<ALDCharacterPlayer>(GetPawn());
}

void ALDPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ALDPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(ULDAbilitySystemComponent* ASC = GetLDAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ALDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void ALDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

#pragma endregion

#pragma region PC-AbilitySystem

ULDAbilitySystemComponent* ALDPlayerController::GetLDAbilitySystemComponent() const
{
	const ALDPlayerState* PS = GetLDPlayerState();
	return CastChecked<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
}

#pragma endregion


ALDPlayerState* ALDPlayerController::GetLDPlayerState() const
{
	return CastChecked<ALDPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

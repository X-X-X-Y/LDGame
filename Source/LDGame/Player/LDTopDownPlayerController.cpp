// Fill out your copyright notice in the Description page of Project Settings.


#include "LDTopDownPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "LDTopDownPlayerState.h"
#include "Character/Hero/LDCharacterHero.h"
#include "Character/Hero/LDPlayerPawn.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameDevUtil/LDLogChannels.h"
#include "GameMode/LDTopGameMode.h"
#include "Input/LDInputComponent.h"


ALDTopDownPlayerController::ALDTopDownPlayerController()
{
}

#pragma region Control UEBeh


void ALDTopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CurrentPlayerPawn = Cast<ALDPlayerPawn>(GetPawn());
	OnPlayerSelectChange.AddDynamic(this, &ThisClass::OnPlayerSelectHeroChange);
}

#pragma endregion

void ALDTopDownPlayerController::OnPlayerSelectHeroChange(EPlayerSelectState NewState)
{
	ALDTopDownPlayerState* PS = GetPlayerState<ALDTopDownPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	ALDTopGameMode* LDGameMode = GetWorld()->GetAuthGameMode<ALDTopGameMode>();
	if (!IsValid(LDGameMode) || !IsValid(CurrentPlayerPawn.Get()))
	{
		UE_LOG(LogLD, Error, TEXT("Check GameMode and DefaultPawn!"))
		return;
	}
	
	switch (NewState)
	{
	case EPlayerSelectState::OnSelectHero:
		if (PS->GetPlayerSelectState() == EPlayerSelectState::OnSelectNone)
		{
			ALDCharacterHero* SelectHero = LDGameMode->GetSelectHeroActor();
			if (SelectHero)
			{
				Possess(SelectHero);
				SetViewTarget(CurrentPlayerPawn.Get());
				PS->SetPlayerSelectState(EPlayerSelectState::OnSelectHero);
				UE_LOG(LogLD, Log, TEXT("OnPlayerSelectHero"));
			}
		}
		break;
	case EPlayerSelectState::OnSelectNone:
		Possess(CurrentPlayerPawn.Get());
		UE_LOG(LogLD, Log, TEXT("On Player Cancel Hero"));
		break;
	}
	
	// ALDTopDownPlayerState* PS = GetPlayerState<ALDTopDownPlayerState>();
	// if (PS)
	// {
	// 	ALDTopGameMode* LDGameMode = GetWorld()->GetAuthGameMode<ALDTopGameMode>();
	// 	if (!IsValid(LDGameMode) || !IsValid(CurrentPlayerPawn))
	// 	{
	// 		UE_LOG(LogLD, Error, TEXT("Check GameMode and DefaultPawn!"))
	// 		return;
	// 	}
	// 	
	// 	switch (PS->GetPlayerSelectState())
	// 	{
	// 	case EPlayerSelectState::OnSelectHero:
	// 		Possess(CurrentPlayerPawn);
	// 		break;
	// 	case EPlayerSelectState::OnSelectNone:
	// 		ALDCharacterHero* ALDCharacterHero = LDGameMode->GetSelectHeroActor();
	// 		if (ALDCharacterHero)
	// 		{
	// 			Possess(ALDCharacterHero);
	// 			SetViewTarget(CurrentPlayerPawn);
	// 			PS->SetPlayerSelectState(EPlayerSelectState::OnSelectHero);
	// 		}
	// 		break;
	// 	}
	// }
}


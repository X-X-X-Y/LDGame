// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"

#include "LDPlayerState.h"
#include "LDTopDownPlayerState.h"
#include "Character/AbilitySystem/LDAbilitySystemComponent.h"
#include "Character/Hero/LDCharacterHero.h"
#include "Character/Hero/LDCharacterPlayer.h"
#include "GameDevUtil/LDLogChannels.h"
#include "GameMode/LDTopGameMode.h"

ALDPlayerController::ALDPlayerController()
{
}

#pragma region PControl-Intreface

void ALDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerPawn = Cast<ALDCharacterPlayer>(GetPawn());
	OnPlayerSelectChange.AddDynamic(this, &ThisClass::OnPlayerSelectHeroChange);
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

#pragma region PC-AbilitySystem

ULDAbilitySystemComponent* ALDPlayerController::GetISAbilitySystemComponent() const
{
	const ALDPlayerState* PS = GetLDPlayerState();
	return CastChecked<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
}

void ALDPlayerController::OnPlayerSelectHeroChange(EPlayerSelectState NewState)
{
	ALDPlayerState* PS = GetPlayerState<ALDPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	ALDTopGameMode* LDGameMode = GetWorld()->GetAuthGameMode<ALDTopGameMode>();
	if (!IsValid(LDGameMode))
	{
		UE_LOG(LogLD, Error, TEXT("Check GameMode and DefaultPawn!"))
		return;
	}

	if (CurrentPlayerPawn.Get() == nullptr)
	{
		CurrentPlayerPawn = Cast<ALDCharacterPlayer>(GetPawn());
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
				CurrentPlayerPawn.Get()->SetActorLocation(SelectHero->GetActorLocation());
				CurrentPlayerPawn.Get()->FollowCharacterHero(SelectHero->GetActorLocation());
				PS->SetPlayerSelectState(EPlayerSelectState::OnSelectHero);
				UE_LOG(LogLD, Log, TEXT("OnPlayerSelectHero"));
			}
		}
		break;
	case EPlayerSelectState::OnSelectNone:
		Possess(CurrentPlayerPawn.Get());
		PS->SetPlayerSelectState(EPlayerSelectState::OnSelectNone);
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

#pragma endregion

ALDPlayerState* ALDPlayerController::GetLDPlayerState() const
{
	return CastChecked<ALDPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

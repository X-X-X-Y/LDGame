// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/LDCharacterPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerState.h"

#pragma region CharacterInterface

ALDCharacterPlayer::ALDCharacterPlayer()
{
	PlayerPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PlayerPawnMovement"));
}

void ALDCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);
}

void ALDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CurrentSelectState = EPlayerSelectState::OnSelectNone;
}

void ALDCharacterPlayer::OnZoomValueChanged()
{
	Super::OnZoomValueChanged();

	if (PlayerPawnMovement)
	{
		PlayerPawnMovement->MaxSpeed = FMath::Lerp(1000, 6000, ZoomCurve->GetFloatValue(ZoomValue));
	}
}

void ALDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->Deactivate();
		GetCharacterMovement()->PrimaryComponentTick.bCanEverTick = false;
	}
}

#pragma endregion

#pragma region PlayerMovement



#pragma endregion

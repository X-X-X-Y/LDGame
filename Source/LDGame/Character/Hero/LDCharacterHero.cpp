// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/LDCharacterHero.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/AbilitySystem/LDAbilitySet.h"
#include "Character/AbilitySystem/LDAbilitySystemComponent.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerController.h"
#include "Player/LDPlayerState.h"
#include "Player/LDTopDownPlayerController.h"
#include "Player/LDTopDownPlayerState.h"

#pragma region ALDCharacter UEBeh

void ALDCharacterHero::BeginPlay()
{
	Super::BeginPlay();
}

void ALDCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ALDCharacterHero::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#pragma endregion


#pragma region Natvie Input

void ALDCharacterHero::OnInputStarted(const FInputActionValue& InputValue)
{
	ALDTopDownPlayerController* LDGamePC = GetController<ALDTopDownPlayerController>();
	if (LDGamePC)
	{
		LDGamePC->StopMovement();
	}
}

void ALDCharacterHero::OnSetDestinationTriggered(const FInputActionValue& Value)
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	ALDPlayerController* LDGamePC = GetController<ALDPlayerController>();
	if (LDGamePC)
	{
		FHitResult Hit;
		bool bHitSuccessful = false;
		bHitSuccessful = LDGamePC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
		if (bHitSuccessful)
		{
			CachedDestination = Hit.Location;
		}
	
		FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
		AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ALDCharacterHero::OnSetDestinationReleased(const FInputActionValue& Value)
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		ALDPlayerController* LDGamePC = GetController<ALDPlayerController>();
		if (LDGamePC)
		{
			// We move there and spawn some particles
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(LDGamePC, CachedDestination);
			// UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}

	FollowTime = 0.f;
}

void ALDCharacterHero::OnHeroCancelSelect(const FInputActionValue& Value)
{
	//再次按空格,PlayControl回到玩家Pawn,停止操控
	ALDPlayerController* PC = Cast<ALDPlayerController>(GetController());
	if (PC)
	{
		PC->OnPlayerSelectChange.Broadcast(EPlayerSelectState::OnSelectNone);
	}
}

#pragma endregion

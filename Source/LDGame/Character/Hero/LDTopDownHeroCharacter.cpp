// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/LDTopDownHeroCharacter.h"

#include "AI/NavigationSystemBase.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"

ALDTopDownHeroCharacter::ALDTopDownHeroCharacter()
{
	CachedDestination = FVector::ZeroVector;

}

#pragma region TopDown Hero Input

void ALDTopDownHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//基础能力
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	TArray<uint32> BindHandles;
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_TD_Move, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_TD_Move, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_TD_Move, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_TD_Move, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);
}

#pragma region TopDown Hero MoveToTarget

void ALDTopDownHeroCharacter::OnInputStarted(const FInputActionValue& Value)
{
	ALDPlayerController* LDGamePC = GetController<ALDPlayerController>();
	if (LDGamePC)
	{
		LDGamePC->StopMovement();
	}
}

void ALDTopDownHeroCharacter::OnSetDestinationTriggered(const FInputActionValue& Value)
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

void ALDTopDownHeroCharacter::OnSetDestinationReleased(const FInputActionValue& Value)
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



#pragma endregion

#pragma endregion

#pragma region Common Functions

#pragma endregion
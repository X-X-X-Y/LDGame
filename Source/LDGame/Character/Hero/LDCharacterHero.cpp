// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/LDCharacterHero.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/AbilitySystem/LDAbilitySet.h"
#include "Character/AbilitySystem/LDAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerController.h"
#include "Player/LDPlayerState.h"
#include "Player/LDTopDownPlayerController.h"
#include "Player/LDTopDownPlayerState.h"
#include "Player/AIControl/LDAIHeroController.h"

#pragma region ALDCharacter UEBeh

ALDCharacterHero::ALDCharacterHero()
{
	ViewFollowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	CameraBoom->SetupAttachment(ViewFollowMesh);
}

void ALDCharacterHero::BeginPlay()
{
	Super::BeginPlay();

	ViewCursorCollision->SetVisibility(false);
	ViewCursorMesh->SetVisibility(false);
}

void ALDCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Input
	if(ALDPlayerController* PC = Cast<ALDPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	UpdateCharacterViewZoom(0.0f);
}

void ALDCharacterHero::UnPossessed()
{
	Super::UnPossessed();

	// if (AIControllerClass.Get())
	// {
	// 	ALDAIHeroController* HeroAIController = Cast<ALDAIHeroController>(AIControllerClass.Get());
	// 	if (HeroAIController)
	// 	{
	// 		HeroAIController->Possess(this);
	// 		HeroAIController->MoveToLocation(CachedDestination);
	// 	}
	// }
}

void ALDCharacterHero::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);
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

void ALDCharacterHero::UpdateCursorPosition(FVector OffsetDirection)
{
	Super::UpdateCursorPosition(OffsetDirection);
	ViewFollowMesh->AddWorldOffset(FVector(OffsetDirection.X, OffsetDirection.Y, 0));
};

FVector ALDCharacterHero::GetHeroTargetLocation()
{
	return CachedDestination;
}

#pragma endregion

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

	// AbilitySystemComponent
	ALDPlayerState* PS = Cast<ALDPlayerState>(GetPlayerState());
	check(PS);

	AbilitySystemComponent = Cast<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(PS,this);

	if(AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
	}

	if(ALDPlayerController* PC = Cast<ALDPlayerController>(NewController))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(HeroMappingContext, 0);
		}
	}
}

void ALDCharacterHero::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//绑定技能输入
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	TArray<uint32> BindHandles;
	LDInputComponent->BindAbilityActions(HeroInputConfig, this, &ThisClass::InputAbilityInputTagPressed,
	&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);

	// 基本输入
	LDInputComponent->BindNativeAction(HeroInputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Started,
	                                   this, &ThisClass::OnInputStarted);
	LDInputComponent->BindNativeAction(HeroInputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Triggered,
	                                   this,
	                                   &ThisClass::OnSetDestinationTriggered);
	LDInputComponent->BindNativeAction(HeroInputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Completed,
	                                   this,
	                                   &ThisClass::OnSetDestinationReleased);
	LDInputComponent->BindNativeAction(HeroInputConfig, LDGameplayTags::InputTag_Hero_Move, ETriggerEvent::Canceled,
	                                   this,
	                                   &ThisClass::OnSetDestinationReleased);
	LDInputComponent->BindNativeAction(HeroInputConfig, LDGameplayTags::InputTag_Player_HSelect, ETriggerEvent::Started,
	                                   this, &ThisClass::OnHeroCancelSelect);
}

#pragma endregion

#pragma region GAS Input
void ALDCharacterHero::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ALDCharacterHero::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
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
	ALDTopDownPlayerController* PC = Cast<ALDTopDownPlayerController>(GetController());
	if (PC)
	{
		PC->OnPlayerSelectChange.Broadcast(EPlayerSelectState::OnSelectNone);
	}
}

#pragma endregion




ULDAbilitySystemComponent* ALDCharacterHero::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

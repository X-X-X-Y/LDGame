// Fill out your copyright notice in the Description page of Project Settings.


#include "LDCharacterBase.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/LDAbilitySet.h"
#include "AbilitySystem/LDAbilitySystemComponent.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerController.h"
#include "Player/LDPlayerState.h"

// Sets default values
ALDCharacterBase::ALDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

#pragma region ~AActor Interface

void ALDCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//绑定技能输入
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	TArray<uint32> BindHandles;
	LDInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
	&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);
	
	//子类这里处理自己的Native Input
}

void ALDCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!Cast<ALDPlayerController>(NewController))
	{
		return;
	}
	

	// AbilitySystemComponent
	ALDPlayerState* PS = Cast<ALDPlayerState>(GetPlayerState());
	if (IsValid(PS))
	{
		AbilitySystemComponent = Cast<ULDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS,this);

		if(AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
		}
	}
}

void ALDCharacterBase::UnPossessed()
{
	Super::UnPossessed();
	
}

void ALDCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//Input
	if(ALDPlayerController* PC = Cast<ALDPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


#pragma endregion

#pragma region GAS Input

void ALDCharacterBase::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ALDCharacterBase::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

#pragma endregion


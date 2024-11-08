// Fill out your copyright notice in the Description page of Project Settings.


#include "LDHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerController.h"

#pragma region UR Behaviour
void ALDHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(ALDPlayerController* PC = Cast<ALDPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	} 
}
#pragma endregion

#pragma region HeroInput

void ALDHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//绑定技能输入
	ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(PlayerInputComponent);
	check(LDInputComponent);

	TArray<uint32> BindHandles;
	LDInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
	&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);
}

void ALDHeroCharacter::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void ALDHeroCharacter::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
}

#pragma endregion

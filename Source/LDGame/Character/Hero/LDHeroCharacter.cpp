// Fill out your copyright notice in the Description page of Project Settings.


#include "LDHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/LDInputComponent.h"
#include "Player/LDPlayerController.h"


ALDHeroCharacter::ALDHeroCharacter()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

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

	//绑定玩家视角输入
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Move, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerMove);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Spin, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerMove);
	LDInputComponent->BindNativeAction(InputConfig, LDGameplayTags::InputTag_Player_Zoom, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerMove);
}

void ALDHeroCharacter::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void ALDHeroCharacter::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
}
#pragma region Player Native Input

void ALDHeroCharacter::OnPlayerMove(const FInputActionValue& InputValue)
{
	ALDPlayerController* PC = Cast<ALDPlayerController>(GetController());
	if (PC)
	{
		const FVector2D Value = InputValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ALDHeroCharacter::OnPlayerSpin(const FInputActionValue& Value)
{
}

void ALDHeroCharacter::OnPlayerZoom(const FInputActionValue& Value)
{
}

#pragma endregion

#pragma endregion

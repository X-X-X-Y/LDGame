// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "LDPlayerState.h"
#include "Character/AbilitySystem/LDAbilitySystemComponent.h"
#include "Character/Hero/LDCharacterHero.h"
#include "Character/Hero/LDCharacterPlayer.h"
#include "GameDevUtil/LDGameplayTags.h"
#include "GameDevUtil/LDLogChannels.h"
#include "GameMode/LDTopGameMode.h"
#include "Input/LDInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALDPlayerController::ALDPlayerController()
{
}

#pragma region APControl-Intreface

void ALDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerPawn = Cast<ALDCharacterPlayer>(GetPawn());
	OnPlayerSelectChange.AddDynamic(this, &ThisClass::OnPlayerSelectHeroChange);

	//PlayerViewPos
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(MoveTrackingTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::MoveTracking),0.02f,true);
	}
	
}

void ALDPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ALDPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(ULDAbilitySystemComponent* ASC = GetLDAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ALDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(ALDCharacterBase* CharacterPawn = Cast<ALDCharacterBase>(InPawn); IsValid(CharacterPawn))
	{
		CharacterPawn->UpdateCharacterViewZoom(0.0f);
	}
}

void ALDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//这里绑定视角输入
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PCMappingContext, 0);
	}

	if (ULDInputComponent* LDInputComponent = Cast<ULDInputComponent>(InputComponent))
	{
		LDInputComponent->BindNativeAction(PCInputConfig, LDGameplayTags::InputTag_Player_Move, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerMove);
		LDInputComponent->BindNativeAction(PCInputConfig, LDGameplayTags::InputTag_Player_Zoom, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerZoom);
		LDInputComponent->BindNativeAction(PCInputConfig, LDGameplayTags::InputTag_Player_Select, ETriggerEvent::Started, this, &ThisClass::OnPlayerSelect);
		LDInputComponent->BindNativeAction(PCInputConfig, LDGameplayTags::InputTag_Player_Select, ETriggerEvent::Completed, this, &ThisClass::OnPlayerSelectComplete);
		LDInputComponent->BindNativeAction(PCInputConfig, LDGameplayTags::InputTag_Player_DMove, ETriggerEvent::Triggered, this, &ThisClass::OnPlayerDragMove);
	}
}

#pragma endregion

#pragma region PC-AbilitySystem

ULDAbilitySystemComponent* ALDPlayerController::GetLDAbilitySystemComponent() const
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

	ALDCharacterHero* SelectHero = LDGameMode->GetSelectHeroActor();
	if (!IsValid(SelectHero) || SelectHero == nullptr)
	{
		//当前场景没有有效的Hero可以选择
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
			Possess(SelectHero);
			SetViewTarget(CurrentPlayerPawn.Get());
			CurrentPlayerPawn.Get()->SetActorLocation(SelectHero->GetActorLocation());
			PS->SetPlayerSelectState(EPlayerSelectState::OnSelectHero);
			UE_LOG(LogLD, Log, TEXT("OnPlayerSelectHero"));
		}
		break;
	case EPlayerSelectState::OnSelectNone:
		Possess(CurrentPlayerPawn.Get());
		PS->SetPlayerSelectState(EPlayerSelectState::OnSelectNone);
		UE_LOG(LogLD, Log, TEXT("On Player Cancel Hero"));
		break;
	}
}

#pragma endregion

#pragma region Player View Functions

void ALDPlayerController::OnPlayerMove(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	if (APawn* ControlledPawn = GetPawn(); IsValid(ControlledPawn))
	{
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			ControlledPawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			ControlledPawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ALDPlayerController::OnPlayerSelect(const FInputActionValue& InputValue)
{
	MouseToGroundPlane(TargetHandle, bIsMousePos);
	bIsLeftMouseButtonDown = true;
}

void ALDPlayerController::OnPlayerSelectComplete(const FInputActionValue& InputValue)
{
	bIsLeftMouseButtonDown = false;
}

void ALDPlayerController::OnPlayerSpin(const FInputActionValue& Value)
{
}

void ALDPlayerController::OnPlayerZoom(const FInputActionValue& InputValue)
{
	ALDCharacterBase* CharacterPawn = GetPawn<ALDCharacterBase>();
	if (IsValid(CharacterPawn))
	{
		CharacterPawn->UpdateCharacterViewZoom(InputValue.Get<float>());
	}
}

void ALDPlayerController::OnPlayerDragMove(const FInputActionValue& InputValue)
{
	if (!bIsLeftMouseButtonDown)
	{
		return;
	}
	
	ALDCharacterBase* CharacterPawn = GetPawn<ALDCharacterBase>();
	if (IsValid(CharacterPawn))
	{
		CharacterPawn->DragCharacterPosition(TargetHandle);
	}
}

void ALDPlayerController::OnPlayerHeroSelect(const FInputActionValue& InputValue)
{
	
}

void ALDPlayerController::MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const
{
	//1-获取相机到选中点的向量
	
	FVector2d CurrentPos;
	FVector WorldPosition;
	FVector WorldDirection;
	float TempValue = 0;
	bIsMouse = false;
	
	if (GetMousePosition(CurrentPos.X, CurrentPos.Y))
	{
		//鼠标位置能够获取
		DeprojectScreenPositionToWorld(CurrentPos.X, CurrentPos.Y, WorldPosition, WorldDirection);
		bIsMouse = true;
	}
	else
	{
		int32 SizeX, SizeY;
		GetViewportSize(SizeX, SizeY);
		FVector2D ViewportSize = FVector2D(SizeX, SizeY);
		CurrentPos = UKismetMathLibrary::Divide_Vector2DVector2D(ViewportSize,FVector2d(2.0,2.0));
		DeprojectScreenPositionToWorld(CurrentPos.X, CurrentPos.Y, WorldPosition, WorldDirection);
		bIsMouse = false;
	}

	FPlane CurrentPlane = UKismetMathLibrary::MakePlaneFromPointAndNormal(FVector(0, 0, 0), FVector(0, 0, 1));
	FVector LineEnd = WorldPosition + (WorldDirection * 100000);

	UKismetMathLibrary::LinePlaneIntersection(WorldPosition, LineEnd, CurrentPlane, TempValue, Intersection);
}

void ALDPlayerController::MoveTracking()
{
	FVector CurrentTargetPos;
	MouseToGroundPlane(CurrentTargetPos, bIsMousePos);
	CurrentTargetPos += FVector(0, 0, 10);

	ALDCharacterBase* CharacterPawn = GetPawn<ALDCharacterBase>();
	if (IsValid(CharacterPawn))
	{
		CharacterPawn->SetCurrentCursorPosition(CurrentTargetPos);
	}
}

#pragma endregion

ALDPlayerState* ALDPlayerController::GetLDPlayerState() const
{
	return CastChecked<ALDPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

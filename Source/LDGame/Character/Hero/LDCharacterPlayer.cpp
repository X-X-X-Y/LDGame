// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/LDCharacterPlayer.h"

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Input/LDInputComponent.h"
#include "LevelActor/LevelInteractable/LDInteractableBuildPlane.h"
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

void ALDCharacterPlayer::UpdateCursorPosition(FVector OffsetDirection)
{
	Super::UpdateCursorPosition(OffsetDirection);

	AddActorWorldOffset(FVector(OffsetDirection.X, OffsetDirection.Y, 0));
}

void ALDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->Deactivate();
		GetCharacterMovement()->PrimaryComponentTick.bCanEverTick = false;
	}

	ViewCursorCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerEntryBuildPlane);
	ViewCursorCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPlayerExitBuildPlane);
}

#pragma endregion

#pragma region BuildPlane

void ALDCharacterPlayer::OnPlayerEntryBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (Cast<ALDInteractableBuildPlane>(OtherActor))
	{
		BuildPlane = Cast<ALDInteractableBuildPlane>(OtherActor);
	}
}

void ALDCharacterPlayer::OnPlayerExitBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BuildPlane.Reset();
	BuildPlane = nullptr;
}

ALDInteractableBuildPlane* ALDCharacterPlayer::GetInteractableBuildPlane() const
{
	return BuildPlane.Get();
}

#pragma endregion

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LDCharacterBase.h"
#include "Player/LDPlayerState.h"
#include "LDCharacterPlayer.generated.h"

enum class EPlayerSelectState : uint8;
struct FInputActionValue;
class UFloatingPawnMovement;
class USphereComponent;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDCharacterPlayer : public ALDCharacterBase
{
	GENERATED_BODY()

public:

	ALDCharacterPlayer();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnZoomValueChanged() override;
	virtual void UpdateCursorPosition(FVector OffsetDirection) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	UFloatingPawnMovement* PlayerPawnMovement;
	
	FTimerHandle MoveTrackingTimerHandle;

private:
	EPlayerSelectState CurrentSelectState = EPlayerSelectState::OnSelectNone;
	bool bIsLeftMouseButtonDown = false;
};

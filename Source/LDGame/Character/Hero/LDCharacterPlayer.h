// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LDCharacterBase.h"
#include "Player/LDPlayerState.h"
#include "LDCharacterPlayer.generated.h"

class ALDInteractableBuildPlane;
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

	ALDInteractableBuildPlane* GetInteractableBuildPlane() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnZoomValueChanged() override;
	virtual void UpdateCursorPosition(FVector OffsetDirection) override;

	UFUNCTION()
	void OnPlayerEntryBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnPlayerExitBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	UFloatingPawnMovement* PlayerPawnMovement;
	
	FTimerHandle MoveTrackingTimerHandle;

private:
	EPlayerSelectState CurrentSelectState = EPlayerSelectState::OnSelectNone;
	bool bIsLeftMouseButtonDown = false;
	TWeakObjectPtr<ALDInteractableBuildPlane> BuildPlane;
};

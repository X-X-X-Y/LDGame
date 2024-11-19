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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	void FollowCharacterHero(FVector HeroLocation);
	
protected:
	virtual void BeginPlay() override;
	
	void OnPlayerMove(const FInputActionValue& InputValue);
	void OnPlayerSelect(const FInputActionValue& InputValue);
	void OnPlayerSelectComplete(const FInputActionValue& InputValue);
	void OnPlayerSpin(const FInputActionValue& Value);
	void OnPlayerZoom(const FInputActionValue& InputValue);
	void OnPlayerDragMove(const FInputActionValue& InputValue);
	void OnPlayerHeroSelect(const FInputActionValue& InputValue);

	void UpdatePlayerViewZoom();

private:
	void MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const;
	void MoveTracking();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	UCurveFloat* ZoomCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	float ZoomValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|Game")
	UStaticMeshComponent* PlayerCursorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|Game")
	USphereComponent* PlayerCursorCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	UFloatingPawnMovement* PlayerPawnMovement;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|Game")
	// USceneComponent* PlayerPawnRoot;
	
	FTimerHandle MoveTrackingTimerHandle;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	float ZoomDirection;
	FVector TargetHandle = FVector::ZeroVector;
	bool bIsMousePos = false;
	EPlayerSelectState CurrentSelectState = EPlayerSelectState::OnSelectNone;
	bool bIsLeftMouseButtonDown = false;
};

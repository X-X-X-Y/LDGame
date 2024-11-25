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

	UFUNCTION()
	void OnPlayerEntryBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnPlayerExitBuildPlane(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnPlayerMove(const FInputActionValue& InputValue);
	void OnPlayerSelect(const FInputActionValue& InputValue);
	void OnPlayerSelectComplete(const FInputActionValue& InputValue);
	void OnPlayerSpin(const FInputActionValue& Value);
	void OnPlayerZoom(const FInputActionValue& InputValue);
	void OnPlayerDragMove(const FInputActionValue& InputValue);
	void OnPlayerHeroSelect(const FInputActionValue& InputValue);
	void OnHeroSelect(const FInputActionValue& InputValue);
	void OnBuildModeChange(const FInputActionValue& InputValue);

	void MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const;
	void MoveTracking();
	void UpdatePlayerViewZoom();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	UFloatingPawnMovement* PlayerPawnMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|View")
	UStaticMeshComponent* ViewCursorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|View")
	USphereComponent* ViewCursorCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|View")
	UCurveFloat* ZoomCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|View")
	float ZoomValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	FTimerHandle MoveTrackingTimerHandle;

private:
	EPlayerSelectState CurrentSelectState = EPlayerSelectState::OnSelectNone;
	bool bIsLeftMouseButtonDown = false;
	TWeakObjectPtr<ALDInteractableBuildPlane> BuildPlane;
	
	float ZoomDirection;
	FVector TargetHandle = FVector::ZeroVector;
	bool bIsMousePos = false;
};

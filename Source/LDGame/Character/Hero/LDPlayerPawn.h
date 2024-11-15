// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LDPawn.h"
#include "LDGame/Character/LDCharacterBase.h"
#include "LDPlayerPawn.generated.h"

class ULDAbilitySet;
class UFloatingPawnMovement;
class USphereComponent;
struct FInputActionValue;
struct FGameplayTag;
class ULDInputConfig;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDPlayerPawn : public ALDPawn
{
	GENERATED_BODY()
	
public:

	ALDPlayerPawn();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
protected:
	virtual void BeginPlay() override;
	// Client only.
	virtual void OnRep_PlayerState() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void OnPlayerMove(const FInputActionValue& InputValue);
	void OnPlayerSelect(const FInputActionValue& InputValue);
	void OnPlayerSpin(const FInputActionValue& Value);
	void OnPlayerZoom(const FInputActionValue& InputValue);
	void OnPlayerDragMove(const FInputActionValue& InputValue);

	void UpdatePlayerViewZoom();

private:
	void MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const;
	void MoveTracking();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* InputConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* DefaultMappingContext;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Abilities")
	ULDAbilitySet* AbilitySet;
	
	FTimerHandle MoveTrackingTimerHandle;

	USceneComponent* PlayerPawnRoot;
	
	TWeakObjectPtr<class ULDAbilitySystemComponent> AbilitySystemComponent;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


	float ZoomDirection;
	FVector TargetHandle = FVector::ZeroVector;
	bool bIsMousePos = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LDGame/Character/LDCharacterBase.h"
#include "LDHeroCharacter.generated.h"

struct FInputActionValue;
struct FGameplayTag;
class ULDInputConfig;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDHeroCharacter : public ALDCharacterBase
{
	GENERATED_BODY()
	
public:

	ALDHeroCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void OnPlayerMove(const FInputActionValue& InputValue);
	void OnPlayerSelect(const FInputActionValue& InputValue);
	void OnPlayerSpin(const FInputActionValue& Value);
	void OnPlayerZoom(const FInputActionValue& InputValue);
	void OnPlayerDragMove(const FInputActionValue& InputValue);

	void UpdatePlayerViewZoom();

private:
	FVector GetMouseToGroundPlane() const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* InputConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	UCurveFloat* ZoomCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Zoom")
	float ZoomValue;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	float ZoomDirection;
	FVector TargetHandle = FVector::ZeroVector;
};

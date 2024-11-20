// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LDCharacterBase.generated.h"

class USphereComponent;
struct FGameplayTag;
class ULDInputConfig;
class ULDAbilitySystemComponent;
class ULDAbilitySet;

UCLASS()
class LDGAME_API ALDCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALDCharacterBase();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	void SetCurrentCursorPosition(const FVector& TargetPos) const;
	void DragCharacterPosition(const FVector& TargetHandle);
	void UpdateCharacterViewZoom(float ZoomDirection);

protected:
	virtual void BeginPlay() override;
	// Client only.
	virtual void OnRep_PlayerState() override;
	virtual void OnZoomValueChanged();
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* InputConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Abilities")
	ULDAbilitySet* AbilitySet;

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
	
	TWeakObjectPtr<class ULDAbilitySystemComponent> AbilitySystemComponent;
};

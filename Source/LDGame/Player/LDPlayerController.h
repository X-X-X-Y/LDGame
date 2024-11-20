// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LDPlayerController.generated.h"

class ALDCharacterPlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLDPlayerSelectChange, const EPlayerSelectState, NewState);

class ALDPlayerState;
class ULDAbilitySystemComponent;
enum class EPlayerSelectState : uint8;
class ULDInputConfig;
struct FInputActionValue;
class ALDPlayerPawn;


UCLASS()
class LDGAME_API ALDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ALDPlayerController();
	
	UFUNCTION(BlueprintCallable, Category = "LDGame|PlayerController")
	ALDPlayerState* GetLDPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "InputSystem|ISAbilitySystemComponent")
	ULDAbilitySystemComponent* GetLDAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "LDGame")
	void OnPlayerSelectHeroChange(EPlayerSelectState NewState);

	void MouseToGroundPlane(FVector& Intersection, bool& bIsMouse) const;
	
protected:
	//~APlayerControl-Start
	virtual void BeginPlay() override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	//~APlayerControl-End

	void OnPlayerMove(const FInputActionValue& InputValue);
	void OnPlayerSelect(const FInputActionValue& InputValue);
	void OnPlayerSelectComplete(const FInputActionValue& InputValue);
	void OnPlayerSpin(const FInputActionValue& Value);
	void OnPlayerZoom(const FInputActionValue& InputValue);
	void OnPlayerDragMove(const FInputActionValue& InputValue);
	void OnPlayerHeroSelect(const FInputActionValue& InputValue);

private:

	void MoveTracking();
	
public:
	FLDPlayerSelectChange OnPlayerSelectChange;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* PCMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* PCInputConfig;
	
private:
	TSoftObjectPtr<ALDCharacterPlayer> CurrentPlayerPawn;
	FVector TargetHandle = FVector::ZeroVector;
	bool bIsMousePos = false;
	bool bIsLeftMouseButtonDown = false;
	FTimerHandle MoveTrackingTimerHandle;
};

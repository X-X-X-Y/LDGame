// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/LDPlayerController.h"
#include "LDTopDownPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLDPlayerSelectChange, const EPlayerSelectState, NewState);

enum class EPlayerSelectState : uint8;
class ULDInputConfig;
struct FInputActionValue;
class ALDPlayerPawn;


UCLASS()
class LDGAME_API ALDTopDownPlayerController : public ALDPlayerController
{
	GENERATED_BODY()

public:
	ALDTopDownPlayerController();

	UFUNCTION(BlueprintCallable, Category = "LDGame")
	void OnPlayerSelectHeroChange(EPlayerSelectState NewState);
	
protected:
	virtual void BeginPlay() override;

public:
	FLDPlayerSelectChange OnPlayerSelectChange;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	class UInputMappingContext* PCMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LDGame|Input")
	ULDInputConfig* InputConfig;
	
private:
	TSoftObjectPtr<ALDPlayerPawn> CurrentPlayerPawn;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelActor/LevelInteractable/LDInteractableBase.h"
#include "LDInteractableBuildPlane.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class LDGAME_API ALDInteractableBuildPlane : public ALDInteractableBase
{
	GENERATED_BODY()

public:
	ALDInteractableBuildPlane();

	FVector GetBuildPlanPointPosition(const FVector& MousePos) const;

protected:
	virtual void BeginPlay() override;

private:
	void InitBuildPlaneMesh();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LDGame|BuildPlane")
	float BoxSlotSize = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|BuildPlane")
	USceneComponent* DefaultRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|BuildPlane")
	UStaticMeshComponent* BuildPlaneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LDGame|BuildPlane")
	UBoxComponent* BuildPlaneBoxCollision;


private:
	// TWeakObjectPtr<UStaticMeshComponent> BuildPlaneMesh;
	// TWeakObjectPtr<UBoxComponent> BuildPlaneBoxCollision;
	FVector BuildPlaneScale = FVector::ZeroVector;;
	FVector BuildPlaneLocation = FVector::ZeroVector;
};

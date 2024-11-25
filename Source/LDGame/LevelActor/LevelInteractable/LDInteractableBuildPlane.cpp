// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor/LevelInteractable/LDInteractableBuildPlane.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameDevUtil/LDLogChannels.h"

ALDInteractableBuildPlane::ALDInteractableBuildPlane()
{
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("BuildPlaneRoot");
	RootComponent = DefaultRoot;

	BuildPlaneBoxCollision = CreateDefaultSubobject<UBoxComponent>("BuildPlaneCollision");
	BuildPlaneBoxCollision->SetupAttachment(RootComponent);
	
	BuildPlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>("BuildPlaneCubeMesh");
	BuildPlaneMesh->SetupAttachment(RootComponent);
	
}

FVector ALDInteractableBuildPlane::GetBuildPlanPointPosition(const FVector MousePos) const
{
	FVector BuildPlanPoint = FVector::ZeroVector;
	BuildPlanPoint.Z = BuildPlaneLocation.Z;
	FVector MouseRelativePos = FVector(MousePos.X - BuildPlaneLocation.X,
	                                   MousePos.Y - BuildPlaneLocation.Y, 0);
	
	FVector PosIndex = FVector(
		MouseRelativePos.X < 0
			? FMath::Floor(MouseRelativePos.X / BoxSlotSize)
			: FMath::CeilToInt(MouseRelativePos.X / BoxSlotSize),
		MouseRelativePos.Y < 0
			? FMath::Floor(MouseRelativePos.Y / BoxSlotSize)
			: FMath::CeilToInt(MouseRelativePos.Y / BoxSlotSize),
		0);

	PosIndex.X == 0 ? 1 : PosIndex.X;
	PosIndex.Y == 0 ? 1 : PosIndex.Y;
	
	BuildPlanPoint = FVector(
		BoxSlotSize * (PosIndex.X - 0.5f * (PosIndex.X < 0 ? -1 : 1)) + BuildPlaneLocation.X,
		BoxSlotSize * (PosIndex.Y - 0.5f * (PosIndex.Y < 0 ? -1 : 1)) + BuildPlaneLocation.Y,
		BuildPlaneLocation.Z + BoxSlotSize);
	// UE_LOG(LogLD, Log, TEXT("Current PosIndex X=%f, Y=%f"),PosIndex.X,PosIndex.Y);
	
	return BuildPlanPoint;
} 

void ALDInteractableBuildPlane::BeginPlay()
{
	Super::BeginPlay();

	InitBuildPlaneMesh();
}

void ALDInteractableBuildPlane::InitBuildPlaneMesh()
{
	if (IsValid(BuildPlaneBoxCollision) && IsValid(BuildPlaneMesh))
	{
		FVector Origin = FVector::ZeroVector;
		GetActorBounds(false, Origin,BuildPlaneScale);
		BuildPlaneBoxCollision->SetBoxExtent(BuildPlaneScale);
		UE_LOG(LogLD, Log, TEXT("CurrentActorScale IS X= %f, Y= %f, Z = %f"), BuildPlaneScale.X, BuildPlaneScale.Y,
			   BuildPlaneScale.Z);
		BuildPlaneLocation = GetActorLocation();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor/LevelInteractable/LDInteractableBase.h"

// Sets default values
ALDInteractableBase::ALDInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALDInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALDInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor/LDCharacterSpawnActor.h"

#include "Character/Hero/LDHeroCharacter.h"
#include "GameDevUtil/LDLogChannels.h"

ALDCharacterSpawnActor::ALDCharacterSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALDCharacterSpawnActor::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Spawn Character Actor

void ALDCharacterSpawnActor::SpawnHeroCharacterActor(TSoftClassPtr<ALDHeroCharacter> HeroCharacter)
{
	if (HeroCharacter == nullptr)
	{
		UE_LOG(LogLDCharacter, Type::Error, TEXT("HeroCharacter is nullptr"));
		return;
	}

	if (UWorld* World = GetWorld(); bHasSpawnedHero == false && World)
	{
		World->SpawnActor<ALDHeroCharacter>(HeroCharacter.Get(), this->GetActorLocation(), this->GetActorRotation());
	}
}

#pragma endregion
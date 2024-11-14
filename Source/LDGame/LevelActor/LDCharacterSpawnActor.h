// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LDCharacterSpawnActor.generated.h"

class ALDHeroCharacter;

UENUM(BlueprintType)
enum class ESpawnCharacterType : uint8
{
	HeroActor,
	EnemyActor
};

UCLASS()
class LDGAME_API ALDCharacterSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALDCharacterSpawnActor();

	void SpawnHeroCharacterActor(TSoftClassPtr<ALDHeroCharacter> HeroCharacter);

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "LDGame|SpawnActor")
	ESpawnCharacterType SpawnCharacterType = ESpawnCharacterType::HeroActor;

	bool bHasSpawnedHero = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

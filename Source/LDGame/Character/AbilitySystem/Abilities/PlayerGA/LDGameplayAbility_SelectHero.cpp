// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilitySystem/Abilities/PlayerGA/LDGameplayAbility_SelectHero.h"

#include "GameDevUtil/LDLogChannels.h"
#include "GameMode/LDTopGameMode.h"
#include "Player/LDTopDownPlayerState.h"

#pragma region Abilities Lifecycle

bool ULDGameplayAbility_SelectHero::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
													   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
													   const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ALDTopDownPlayerState* PS = ActorInfo->PlayerController->GetPlayerState<ALDTopDownPlayerState>();
	if (IsValid(PS))
	{
		return  PS->CheckHasSelectHero();
	}
	
	return false;
}

void ULDGameplayAbility_SelectHero::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


void ULDGameplayAbility_SelectHero::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               bool bReplicateEndAbility, bool bWasCancelled)
{
	ALDTopDownPlayerState* PS = ActorInfo->PlayerController->GetPlayerState<ALDTopDownPlayerState>();
	if (PS)
	{
		PS->SetHasSelectHero(false);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion

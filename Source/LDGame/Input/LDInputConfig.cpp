// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/LDInputConfig.h"

const UInputAction* ULDInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FLDInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogInput, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}

const UInputAction* ULDInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FLDInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogInput, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/LDInputComponent.h"

#include "LDInputConfig.h"

ULDInputComponent::ULDInputComponent()
{
}

void ULDInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}


// Copyright 2025 Kalp Games, All rights reserved.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	for (FAuraInputAction InputAction : AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.GameplayTag == InputTag) return InputAction.InputAction;
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find input tag"));
	}
	return nullptr;
}

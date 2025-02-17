// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "AuraAbilitySystemTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}
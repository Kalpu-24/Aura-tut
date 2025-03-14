// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level) const;
	virtual FString GetNextLevelDescription(int32 Level) const;
	static FString GetLockedDescription(int32 Level);
};

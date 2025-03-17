// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bPitchOverride, const float PitchOverride, AActor* HomingTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float ProjectileSpread = 90.0f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float MinHominAcceleration = 1600.0f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float MaxHominAcceleration = 3200.0f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	bool bLaunchHomingProjectile = true;
};

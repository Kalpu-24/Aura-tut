// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bPitchOverride, const float PitchOverride)
{
	if (const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); !bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bPitchOverride)
	{
		Rotator.Pitch = PitchOverride;
	}
		
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotator.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	Projectile->FinishSpawning(SpawnTransform);
	
}

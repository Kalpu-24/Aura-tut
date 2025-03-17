// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level) 
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
 
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
 			
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
 
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
 
			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"
 
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
 
			// Number of FireBolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"
 
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
 
			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
 		
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL: </>\n\n"
 
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
 
			// Number of FireBolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"
 
			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),
 
			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	const bool bPitchOverride, const float PitchOverride, AActor* HomingTarget)
{
	if (const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); !bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bPitchOverride) Rotator.Pitch = PitchOverride;

	// NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	const FVector Forward = Rotator.Vector();
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
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
	
}

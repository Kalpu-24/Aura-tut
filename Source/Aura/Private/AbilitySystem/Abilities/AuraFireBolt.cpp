// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

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
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.0f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.0f, FVector::UpVector);
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetOwningActorFromActorInfo(), SocketLocation,
		SocketLocation + Direction * 100.f, 5, FLinearColor::Red, 120, 1);
		}
	}
	else
	{
		//Single Projectile
	}
}

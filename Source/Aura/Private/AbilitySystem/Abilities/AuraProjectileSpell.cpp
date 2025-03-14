// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"


FString UAuraProjectileSpell::GetDescription(int32 Level) const
{
	const int32 Damage = DamageTypes[TAG_DamageType_Fire].GetValueAtLevel(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches a bolt of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), Damage, Level);
	}
	else
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level) const
{
	const int32 Damage = DamageTypes[TAG_DamageType_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL: </>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	if (const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); !bIsServer) return;


		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		const FRotator Rotator = (ProjectileTargetLocation - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		
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

		const UAbilitySystemComponent* SourceAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceAsc->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		const FGameplayEffectSpecHandle SpecHandle = SourceAsc->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		for (auto DamageType : DamageTypes)
		{
			const float ScaledDamage = DamageType.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType.Key, ScaledDamage);
		}
		
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	
}

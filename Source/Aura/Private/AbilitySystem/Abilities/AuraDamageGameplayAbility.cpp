// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,
 	FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride,
 	bool bOverrideDeathImpulse, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
 {
 	FDamageEffectParams Params;
 	Params.WorldContextObject = GetAvatarActorFromActorInfo();
 	Params.DamageGameplayEffectClass = DamageEffectClass;
 	Params.SourceAsc = GetAbilitySystemComponentFromActorInfo();
 	Params.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
 	Params.BaseDamge = Damage.GetValueAtLevel(GetAbilityLevel());
 	Params.AbilityLevel = GetAbilityLevel();
 	Params.DamageType = DamageType;
 	Params.DebuffChance = DebuffChance;
 	Params.DebuffDamage = DebuffDamage;
 	Params.DebuffDuration = DebuffDuration;
 	Params.DebuffFrequency = DebuffFrequency;
 	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
 	Params.KnockbackForce = KnockbackForce;
 	Params.KnockbackChance = KnockbackChance;
 
 	if (IsValid(TargetActor))
 	{
 		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
 		if (bOverridePitch)
 		{
 			Rotation.Pitch = PitchOverride;
 		}
 		const FVector ToTarget = Rotation.Vector();
 		if (!bOverrideKnockbackDirection)
 		{
 			Params.KnockbackVector = ToTarget * KnockbackForce;
 		}
 		if (!bOverrideDeathImpulse)
 		{
 			Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
 		}
 	}
 	
 	
 	if (bOverrideKnockbackDirection)
 	{
 		KnockbackDirectionOverride.Normalize();
 		Params.KnockbackVector = KnockbackDirectionOverride * KnockbackForce;
 		if (bOverridePitch)
 		{
 			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
 			KnockbackRotation.Pitch = PitchOverride;
 			Params.KnockbackVector = KnockbackRotation.Vector() * KnockbackForce;
 		}
 	}
 
 	if (bOverrideDeathImpulse)
 	{
 		DeathImpulseDirectionOverride.Normalize();
 		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
 		if (bOverridePitch)
 		{
 			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
 			DeathImpulseRotation.Pitch = PitchOverride;
 			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
 		}
 	}
 	
 	if (bIsRadialDamage)
 	{
 		Params.bIsRadialDamage = bIsRadialDamage;
 		Params.RadialDamageOrigin = InRadialDamageOrigin;
 		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
 		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
 	}
 	return Params;
 }

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num()-1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}

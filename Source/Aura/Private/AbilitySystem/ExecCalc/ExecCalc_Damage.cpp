// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct FAuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireRes)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneRes)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningRes)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalRes)

	FAuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockRate, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalRes, Target, false);
	}
};

static  const FAuraDamageStatics& DamageStatics()
{
	static FAuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritResDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters, TMap<FGameplayTag, FGameplayTag> DamageToResistanceTags) const
{
	// debuff
	for (auto Pair: UAuraAbilitySystemLibrary::GetDamageTypesToDebuffTags())
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag DebuffTypeTag = Pair.Value;
		const float TypeDamage =  Spec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);
		if (TypeDamage > -.5f)
		{
			//Determine if there was successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(TAG_Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = DamageToResistanceTags[DamageTypeTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceTag], EvaluateParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);

			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuffed = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuffed)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetEffectContext();

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, bDebuffed);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageTypeTag);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(TAG_Debuff_Damage, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(TAG_Debuff_Frequency, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(TAG_Debuff_Duration, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
		
	TagsToCaptureDefs.Add(TAG_Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Secondary_BlockRate, DamageStatics().BlockRateDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Secondary_CritRate, DamageStatics().CritRateDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Secondary_CritRes, DamageStatics().CritResDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Secondary_CritDamage, DamageStatics().CritDamageDef);

	TagsToCaptureDefs.Add(TAG_Attributes_Resistance_Arcane, DamageStatics().ArcaneResDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Resistance_Fire, DamageStatics().FireResDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Resistance_Lightning, DamageStatics().LightningResDef);
	TagsToCaptureDefs.Add(TAG_Attributes_Resistance_Physical, DamageStatics().PhysicalResDef);
	
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceAsc? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAsc? TargetAsc->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceActor->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceActor);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetActor->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetActor);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = Spec.GetEffectContext();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	TMap<FGameplayTag, FGameplayTag> DamageToResistanceTags = UAuraAbilitySystemLibrary::GetDamageToResistanceTags();
	DetermineDebuff(ExecutionParams, TagsToCaptureDefs, Spec, EvaluateParameters, DamageToResistanceTags);
	
	float Damage = 0.f;
	// Now use DamageTypeTags as needed
	for (auto Pair : DamageToResistanceTags) {

		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("Resistance Tag %s not found in DamageStatics"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float Damagetype = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		if (Damagetype <= 0.f)
		{
			continue;
		}
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp<float>(Resistance,0.f, 100.f);

		Damagetype *= (100.f - Resistance) / 100.f;

		if (UAuraAbilitySystemLibrary::IsRadialDamage(ContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
			{
				CombatInterface->GetOnDamageSignature().AddLambda(
					[&](float DamageAmount)
					{
						Damagetype = DamageAmount;
					}
				);
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetActor,
				Damagetype,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(ContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(ContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(ContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceActor,
				nullptr);
		}
		
		Damage += Damagetype;
	}
	
	// Capture Block Chance on Target and Determine If There was a successful block
	float TargetBlockRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockRateDef, EvaluateParameters, TargetBlockRate);
	TargetBlockRate = FMath::Max<float>(0.f, TargetBlockRate);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockRate;
	// If block , halve the Damage
	Damage = bBlocked? Damage / 2.f : Damage;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(ContextHandle, bBlocked);

	// Armor stuff
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritRateDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritResDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CritRes"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(ContextHandle, bCriticalHit);
	
	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	
	FGameplayModifierEvaluatedData EvaluatedData = FGameplayModifierEvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

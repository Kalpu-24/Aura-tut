// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UAuraAttributeSet::UAuraAttributeSet(){
	InitHealth(50.f);
	InitMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalRes, COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

inline void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesEnhanced& Props)
{
	Props.ContextHandle = Data.EffectSpec.GetContext();
	Props.SourceProperties->AbilitySystemComponent = Props.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceProperties->AbilitySystemComponent) && Props.SourceProperties->AbilitySystemComponent->AbilityActorInfo.IsValid() && Props.SourceProperties->AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceProperties->AvatarActor = Props.SourceProperties->AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		Props.SourceProperties->Controller = Props.SourceProperties->AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceProperties->Controller == nullptr && Props.SourceProperties->AvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceProperties->AvatarActor))
			{
				Props.SourceProperties->Controller = Pawn->GetController();
			}
		}
		if (Props.SourceProperties->Controller)
		{
			Props.SourceProperties->Character = Cast<ACharacter>(Props.SourceProperties->Controller->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetProperties->AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetProperties->Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetProperties->Character = Cast<ACharacter>(Props.TargetProperties->AvatarActor);
		Props.TargetProperties->AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetProperties->AvatarActor);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectPropertiesEnhanced& Props, const float Damage, const bool bIsBlockedHit, const bool bIsCriticalHit) const
{
	if (Props.SourceProperties->Character != Props.TargetProperties->Character)
	{
		if (AAuraPlayerController* Pc = Cast<AAuraPlayerController>(Props.SourceProperties->Controller))
		{
			Pc->ShowDamageNumber(Damage, Props.TargetProperties->Character, bIsBlockedHit, bIsCriticalHit);
			if (AAuraPlayerController* ServerPc = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(Props.SourceProperties->Controller, 0)))
			{
				if (ServerPc != Pc)
				{
					ServerPc->ShowDamageNumber(Damage, Props.TargetProperties->Character, bIsBlockedHit, bIsCriticalHit);
				}
				return;
			}
		}
		if (AAuraPlayerController* ServerPc = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(Props.TargetProperties->Controller, 0)))
		{
			ServerPc->ShowDamageNumber(Damage, Props.TargetProperties->Character, bIsBlockedHit, bIsCriticalHit);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectPropertiesEnhanced& Props)
{
	if (Props.TargetProperties->Character->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetProperties->Character);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetProperties->Character);

		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetProperties->Character, TargetClass, TargetLevel);

		FGameplayEventData Payload;
		Payload.EventTag = TAG_Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceProperties->Character, TAG_Attributes_Meta_IncomingXP, Payload);
	}
	
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::HandleDebuff(const FEffectPropertiesEnhanced& Props)
{
	//Handle Debuff
	FGameplayEffectContextHandle EffectContext = Props.SourceProperties->AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceProperties->AvatarActor);

	const FGameplayTag DamageTypeTag = UAuraAbilitySystemLibrary::GetDamageType(Props.ContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.ContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.ContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.ContextHandle);
			
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageTypeTag.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	Effect->Period = DebuffFrequency;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToDebuffTagMap = UAuraAbilitySystemLibrary::GetDamageTypesToDebuffTags();

	UTargetTagsGameplayEffectComponent& TargetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTags;
	InheritedTags.Added.AddTag(DamageTypeToDebuffTagMap[DamageTypeTag]);
	TargetTagsComponent.SetAndApplyTargetTagChanges(InheritedTags);

	for (auto Name : Effect->GetGrantedTags())
	{
		UE_LOG(LogAura, Log, TEXT("Granted Tag: %s"), *Name.ToString());
	}

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	int32 Index = Effect->Modifiers.Num();
	Index = Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	FGameplayEffectSpec* Spec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);
	if (Spec)
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(Spec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageTypeTag));
		AuraContext->SetDamageType(DebuffDamageType);
	}

	Props.TargetProperties->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectPropertiesEnhanced& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		if (NewHealth <= 0.f)//bFatal
		{
			//TO DO: Handle Death Impulse
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetProperties->AvatarActor))
			{
				CombatInterface->Die();
			}
			SendXPEvent(Props);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(TAG_Effect_HitReact);
			Props.TargetProperties->AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		}
		const bool bIsDamageHit = UAuraAbilitySystemLibrary::IsBlockedHit(Props.ContextHandle);
		const bool bIsCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.ContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bIsDamageHit, bIsCriticalHit);
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.ContextHandle))
		{
			HandleDebuff(Props);
		}
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectPropertiesEnhanced& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	if (Props.SourceProperties->Character->Implements<UPlayerInterface>() && Props.SourceProperties->Character->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceProperties->Character);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceProperties->Character);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceProperties->Character, CurrentXP + LocalIncomingXP);

		if (const int32 NumOfLevelUps = NewLevel - CurrentLevel; NumOfLevelUps > 0)
		{
			const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceProperties->Character, CurrentLevel);
			const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceProperties->Character, CurrentLevel);

			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceProperties->Character, NumOfLevelUps);
				
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceProperties->Character, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceProperties->Character, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceProperties->Character);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceProperties->Character, LocalIncomingXP);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectPropertiesEnhanced Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetProperties->Character->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetProperties->Character)) return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

//Primary Attributes
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

// Secondary Attributes
void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockRate(const FGameplayAttributeData& OldBlockRate) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockRate, OldBlockRate);
}

void UAuraAttributeSet::OnRep_CritRate(const FGameplayAttributeData& OldCritRate) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritRate, OldCritRate);
}

void UAuraAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritDamage, OldCritDamage);
}

void UAuraAttributeSet::OnRep_CritRes(const FGameplayAttributeData& OldCritRes) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritRes, OldCritRes);
}

void UAuraAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegen, OldHealthRegen);
}

void UAuraAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegen, OldManaRegen);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_FireRes(const FGameplayAttributeData& OldFireRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireRes, OldFireRes);
}

void UAuraAttributeSet::OnRep_ArcaneRes(const FGameplayAttributeData& OldArcaneRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneRes, OldArcaneRes);
}

void UAuraAttributeSet::OnRep_LightningRes(const FGameplayAttributeData& OldLightningRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningRes, OldLightningRes);
}

void UAuraAttributeSet::OnRep_PhysicalRes(const FGameplayAttributeData& OldPhysicalRes) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalRes, OldPhysicalRes);
}

// Vital Attributes
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}
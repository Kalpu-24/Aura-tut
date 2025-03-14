// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

class UAuraGameplayAbility;

void UOverlayWidgetController::BroadCastInitialValues()
{
	OnChangeHealth.Broadcast(GetAuraAS()->GetHealth());
	OnChangeMaxHealth.Broadcast(GetAuraAS()->GetMaxHealth());
	OnChangeMana.Broadcast(GetAuraAS()->GetMana());
	OnChangeMaxMana.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindLambdasToDependencies()
{
	GetAuraPS()->OnXpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){OnChangeHealth.Broadcast(Data.NewValue);}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){OnChangeMaxHealth.Broadcast(Data.NewValue);}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetAuraAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){OnChangeMana.Broadcast(Data.NewValue);}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetAuraAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){OnChangeMaxMana.Broadcast(Data.NewValue);}
	);

	if (GetAuraASC())
	{
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		GetAuraASC()->AbilityCommittedCallbacks.AddLambda(
			[this](UGameplayAbility* Ability)
			{
				OnAbilityCommited.Broadcast(Ability->GetCurrentAbilitySpec()->Ability->AbilityTags.First(), Ability->GetCooldownTimeRemaining());
			}
		);
		
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					if (FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message")); Tag.MatchesTag(MessageTag)){
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRow.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP)
{
	ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("LevelUpInfo is not set in the player state"));
	
	int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num()-1;
	
	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
		OnXpPercentChange.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag)
{
	FAuraAbilityInfo LastSlotinfo;
	LastSlotinfo.StatusTag = TAG_Abilities_Status_Unlocked;
	LastSlotinfo.InputTag = PrevSlotTag;
	LastSlotinfo.AbilityTag = TAG_Abilities_Types_None;
	AbilityInfoDelegate.Broadcast(LastSlotinfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);
}

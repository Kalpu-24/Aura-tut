// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BindLambdasToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.StatusTag = StatusTag;
				bool bSpendPointsButtonEnabled = false;
				bool bEquipButtonEnabled = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bSpendPointsButtonEnabled, bEquipButtonEnabled);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bSpendPointsButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
			}
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;
			bool bSpendPointsButtonEnabled = false;
			bool bEquipButtonEnabled = false;
			ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bSpendPointsButtonEnabled, bEquipButtonEnabled);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bSpendPointsButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::BroadCastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitForEquipButtonDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTagExact(TAG_Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = TAG_Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;
	bool bSpendPointsButtonEnabled = false;
	bool bEquipButtonEnabled = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bSpendPointsButtonEnabled, bEquipButtonEnabled);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bSpendPointsButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints,
	bool& bSpendPointsButtonEnabled, bool& bEquipButtonEnabled)
{
	bSpendPointsButtonEnabled = false;
	bEquipButtonEnabled = false;
	if (AbilityStatus.MatchesTagExact(TAG_Abilities_Status_Equipped))
	{
		bEquipButtonEnabled = true;
		if (SpellPoints > 0)
		{
			bSpendPointsButtonEnabled = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(TAG_Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bSpendPointsButtonEnabled = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(TAG_Abilities_Status_Unlocked))
	{
		bEquipButtonEnabled = true;
		if (SpellPoints > 0)
		{
			bSpendPointsButtonEnabled = true;
		}
	}
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	WaitForEquipButtonDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
}

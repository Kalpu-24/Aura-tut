// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

class UAuraGameplayAbility;

void UOverlayWidgetController::BroadCastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnChangeHealth.Broadcast(AuraAttributeSet->GetHealth());
	OnChangeMaxHealth.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnChangeMana.Broadcast(AuraAttributeSet->GetMana());
	OnChangeMaxMana.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindLambdasToDependencies()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	AuraPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		}
	);
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){OnChangeHealth.Broadcast(Data.NewValue);}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){OnChangeMaxHealth.Broadcast(Data.NewValue);}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){OnChangeMana.Broadcast(Data.NewValue);}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){OnChangeMaxMana.Broadcast(Data.NewValue);}
	);

	if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraAsc->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(AuraAsc);
		}
		else
		{
			AuraAsc->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}

		AuraAsc->AbilityCommittedCallbacks.AddLambda(
			[this](UGameplayAbility* Ability)
			{
				OnAbilityCommited.Broadcast(Ability);
			}
		);
		
		AuraAsc->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAsc)
{
	if (!AuraAsc->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UAuraAbilitySystemComponent::GetAbilityTagFromAbilitySpec(AbilitySpec));
			Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromAbilitySpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	AuraAsc->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP) const
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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

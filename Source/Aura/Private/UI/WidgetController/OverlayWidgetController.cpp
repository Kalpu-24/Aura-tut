// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

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

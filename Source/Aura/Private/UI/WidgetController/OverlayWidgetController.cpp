// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnChangeHealth.Broadcast(AuraAttributeSet->GetHealth());
	OnChangeMaxHealth.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnChangeMana.Broadcast(AuraAttributeSet->GetMana());
	OnChangeMaxMana.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnChangeHealth.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnChangeMaxHealth.Broadcast(Data.NewValue);
}

inline void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnChangeMana.Broadcast(Data.NewValue);
}

inline void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnChangeMaxMana.Broadcast(Data.NewValue);
}

// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindLambdasToDependencies()
{
	for (FAuraAttributeInfo AttributeInfoItem : AttributeInfo->AttributeInfos)
	{
		GetAuraASC()->GetGameplayAttributeValueChangeDelegate(AttributeInfoItem.Attribute).AddLambda(
			[this, AttributeInfoItem](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = AttributeInfoItem;
				BroadCastAttributeInfo(Info);
			}
		);
	}
	
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 AttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast(AttributePoints);
		}
	);
}

void UAttributeMenuWidgetController::BroadCastAttributeInfo(FAuraAttributeInfo& Info)
{
	Info.AttributeValue = Info.Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);

	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	for (FAuraAttributeInfo& AttributeInfoItem : AttributeInfo->AttributeInfos)
	{
		BroadCastAttributeInfo(AttributeInfoItem);
	}
}

// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindLambdasToDependencies()
{
	for (FAuraAttributeInfo& AttributeInfoItem : AttributeInfo->AttributeInfos)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeInfoItem.Attribute).AddLambda(
			[this, AttributeInfoItem](const FOnAttributeChangeData& Data){AttributeInfoDelegate.Broadcast(AttributeInfoItem);}
		);
	}
}

void UAttributeMenuWidgetController::BroadCastAttributeInfo(FAuraAttributeInfo& Info) const
{
	Info.AttributeValue = Info.Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	for (FAuraAttributeInfo& AttributeInfoItem : AttributeInfo->AttributeInfos)
	{
		BroadCastAttributeInfo(AttributeInfoItem);
	}
}

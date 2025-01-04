// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindLambdasToDependencies()
{

}

void UAttributeMenuWidgetController::BroadCastAttributeInfo(FAuraAttributeInfo& Info)
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

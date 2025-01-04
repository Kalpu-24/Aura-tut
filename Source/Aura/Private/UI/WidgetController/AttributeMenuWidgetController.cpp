// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindLambdasToDependencies()
{

}

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	const UAuraAttributeSet* As = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(TAG_Attributes_Primary_Strength);
	Info.AttributeValue = As->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}
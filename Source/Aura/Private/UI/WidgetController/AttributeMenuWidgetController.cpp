// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindLambdasToDependencies()
{
	for (FAuraAttributeInfo& AttributeInfoItem : AttributeInfo->AttributeInfos)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeInfoItem.Attribute).AddLambda(
			[this, AttributeInfoItem](const FOnAttributeChangeData& Data){AttributeInfoDelegate.Broadcast(AttributeInfoItem);}
		);
	}

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 AttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast(AttributePoints);
		}
	);
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

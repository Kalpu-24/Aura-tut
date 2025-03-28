// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString NewName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, NewName);
}

void UMVVM_LoadSlot::SetPlayerName(FString NewName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewName);
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

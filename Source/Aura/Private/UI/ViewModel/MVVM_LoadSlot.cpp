// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	//TODO:SEt slot status
	SetWidgetSwitcherIndex.Broadcast(2);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString NewName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, NewName);
}

void UMVVM_LoadSlot::SetPlayerName(FString NewName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewName);
}

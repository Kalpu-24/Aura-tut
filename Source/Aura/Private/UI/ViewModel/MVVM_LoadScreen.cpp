// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);

	Mvvm_LoadSlots.Add(0, LoadSlot_0);
	Mvvm_LoadSlots.Add(1, LoadSlot_1);
	Mvvm_LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotFromIndex(int32 Index) const
{
	return Mvvm_LoadSlots.FindChecked(Index);
}

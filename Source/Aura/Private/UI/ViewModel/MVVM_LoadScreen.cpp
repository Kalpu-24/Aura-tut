// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);

	LoadSlot_0->SetLoadSlotName(FString("SaveGame_0")); 
	LoadSlot_1->SetLoadSlotName(FString("SaveGame_1"));
	LoadSlot_2->SetLoadSlotName(FString("SaveGame_2"));
	
	Mvvm_LoadSlots.Add(0, LoadSlot_0);
	Mvvm_LoadSlots.Add(1, LoadSlot_1);
	Mvvm_LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotFromIndex(int32 Index) const
{
	return Mvvm_LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	Mvvm_LoadSlots[Slot]->PlayerName = EnteredName;

	GameMode->SaveSlotData(Mvvm_LoadSlots[Slot], Slot);
	Mvvm_LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	Mvvm_LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
}

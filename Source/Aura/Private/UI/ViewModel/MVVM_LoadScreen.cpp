// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
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

	LoadSlot_0->SlotIndex = 0;
	LoadSlot_1->SlotIndex = 1;
	LoadSlot_2->SlotIndex = 2;
	
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
	if (!IsValid(GameMode))
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Please switch to Single Player"));
		return;
	}
	Mvvm_LoadSlots[Slot]->SetMapName(GameMode->DefaultMapName);
	Mvvm_LoadSlots[Slot]->SetPlayerName(EnteredName);
	Mvvm_LoadSlots[Slot]->SetPlayerLevel(1);
	Mvvm_LoadSlots[Slot]->SlotStatus = Taken;
	Mvvm_LoadSlots[Slot]->PlayerStartTag = GameMode->DefaultPlayerStartTag;
	Mvvm_LoadSlots[Slot]->MapAssetName = GameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
	GameMode->SaveSlotData(Mvvm_LoadSlots[Slot], Slot);
	Mvvm_LoadSlots[Slot]->InitializeSlot();

	UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GameMode->GetGameInstance());
	GameInstance->LoadSlotName = Mvvm_LoadSlots[Slot]->GetLoadSlotName();
	GameInstance->LoadSlotIndex = Mvvm_LoadSlots[Slot]->SlotIndex;
	GameInstance->PlayerStartTag = GameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	Mvvm_LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const auto LoadSlot : Mvvm_LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = Mvvm_LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteSlotButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	if (IsValid(SelectedSlot))
	{
		AuraGameMode->TravelToMap(SelectedSlot);
	}
}


void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode)) return;
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : Mvvm_LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
 
		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
 
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->InitializeSlot();
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
	}
}

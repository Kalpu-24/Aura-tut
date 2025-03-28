// Copyright 2025 Kalp Games, All rights reserved.

#pragma once
 
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"
 
/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
 
	UPROPERTY()
	FName PlayerStartTag = FName();
 
	UPROPERTY()
	FString LoadSlotName = FString();
 
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
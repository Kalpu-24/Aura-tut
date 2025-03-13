// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindLambdasToDependencies() override;
	virtual void BroadCastInitialValues() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangeSignature SpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bSpendPointsButtonEnabled, bool& bEquipButtonEnabled);
};

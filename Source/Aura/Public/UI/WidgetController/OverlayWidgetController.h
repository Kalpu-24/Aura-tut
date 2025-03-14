// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

struct FGameplayTag;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCommited, FGameplayTag, AbilityTag, float, Cooldown);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadCastInitialValues() override;
	virtual void BindLambdasToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangeSignature OnChangeHealth;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangeSignature OnChangeMaxHealth;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangeSignature OnChangeMana;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangeSignature OnChangeMaxMana;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRow;

	UPROPERTY(BlueprintAssignable, Category="GAS|Ability")
	FOnAbilityCommited OnAbilityCommited;

	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangeSignature OnXpPercentChange;

	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangeSignature OnPlayerLevelChangeDelegate;
 
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnXPChanged(int32 NewXP);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}

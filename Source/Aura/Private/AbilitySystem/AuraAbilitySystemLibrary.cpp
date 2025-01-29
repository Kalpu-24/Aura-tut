// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "HUD/AuraHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
	if (APlayerController* PlayerController = WorldContext->GetWorld()->GetFirstPlayerController())
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams  WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContext)
{
	if (APlayerController* PlayerController = WorldContext->GetWorld()->GetFirstPlayerController())
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams  WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContext, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* Asc)
{
	const AActor* AvatarActor = Asc->GetAvatarActor();
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext);
	if (ClassInfo == nullptr) return;
	
	const FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDeafultInfo(CharacterClass);
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = Asc->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryEffectSpecHandle = Asc->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle SecondaryEffectSpecHandle = Asc->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, PrimaryAttributeContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle VitalEffectSpecHandle = Asc->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, PrimaryAttributeContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldObject, UAbilitySystemComponent* Asc)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldObject);
	if (ClassInfo == nullptr) return;
	
	for (TSubclassOf CommonAbility : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		Asc->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldObject)
{
	const AAuraGameModeBase* AuraGmb = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));
	if (AuraGmb == nullptr) return nullptr;
	return AuraGmb->CharacterClassInfo;
}

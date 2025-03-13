// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/AbilityInfo.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|WidgetController", meta=(DefaultToSelf="WorldContext"))
	static bool MakeWCParams(const UObject* WorldContext, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|WidgetController", meta=(DefaultToSelf="WorldContext"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|WidgetController", meta=(DefaultToSelf="WorldContext"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|WidgetController", meta=(DefaultToSelf="WorldContext"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContext, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* Asc);

	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldObject, UAbilitySystemComponent* Asc, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldObject);

	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsInBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsInCriticalHit);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|GameplayTags")
	static TMap<FGameplayTag, FGameplayTag> GetDamageToResistanceTags();

	UFUNCTION(BlueprintCallable, Category="AuraABilitySystem|Gameplay Mechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContext, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereLocation);

	UFUNCTION(BlueprintPure, Category="AuraABilitySystem|Gameplay Mechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContext, ECharacterClass CharacterClass, int32 CharacterLevel);
};

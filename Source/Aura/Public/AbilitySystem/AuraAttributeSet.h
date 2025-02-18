// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
 
	UPROPERTY(Transient)
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
 
	UPROPERTY(Transient)
	AActor* AvatarActor = nullptr;
 
	UPROPERTY(Transient)
	AController* Controller = nullptr;
 
	UPROPERTY(Transient)
	ACharacter* Character =nullptr;
};
 
USTRUCT()
struct FEffectPropertiesEnhanced
{
	GENERATED_BODY()
 
	FEffectPropertiesEnhanced()
	{
		SourceProperties = MakeShared<FEffectProperties>();
		TargetProperties = MakeShared<FEffectProperties>();
	}
    
	FGameplayEffectContextHandle ContextHandle;
    
	TSharedPtr<FEffectProperties> SourceProperties;
 
	TSharedPtr<FEffectProperties> TargetProperties;
    
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	/*
	 * Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Resilience, Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Vigor, Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	/*
	 * Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ArmorPenetration, Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BlockRate, Category="Secondary Attributes")
	FGameplayAttributeData BlockRate;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CritRate, Category="Secondary Attributes")
	FGameplayAttributeData CritRate;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CritDamage, Category="Secondary Attributes")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CritRes, Category="Secondary Attributes")
	FGameplayAttributeData CritRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_HealthRegen, Category="Secondary Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ManaRegen, Category="Secondary Attributes")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegen);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxMana, Category="Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_FireRes, Category="Secondary Attributes")
	FGameplayAttributeData FireRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireRes);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ArcaneRes, Category="Secondary Attributes")
	FGameplayAttributeData ArcaneRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneRes);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_LightningRes, Category="Secondary Attributes")
	FGameplayAttributeData LightningRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningRes);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_PhysicalRes, Category="Secondary Attributes")
	FGameplayAttributeData PhysicalRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalRes);


	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockRate(const FGameplayAttributeData& OldBlockRate) const;

	UFUNCTION()
	void OnRep_CritRate(const FGameplayAttributeData& OldCritRate) const;

	UFUNCTION()
	void OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const;

	UFUNCTION()
	void OnRep_CritRes(const FGameplayAttributeData& OldCritRes) const;

	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;

	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	UFUNCTION()
	void OnRep_FireRes(const FGameplayAttributeData& OldFireRes) const;

	UFUNCTION()
	void OnRep_ArcaneRes(const FGameplayAttributeData& OldArcaneRes) const;

	UFUNCTION()
	void OnRep_LightningRes(const FGameplayAttributeData& OldLightningRes) const;

	UFUNCTION()
	void OnRep_PhysicalRes(const FGameplayAttributeData& OldPhysicalRes) const;
	
	/*
	 * Vital Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	/*
	 * Meta Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesEnhanced& Props);
	void ShowFloatingText(const FEffectPropertiesEnhanced& Props, float Damage, bool bIsBlockedHit, bool bIsCriticalHit) const;
};
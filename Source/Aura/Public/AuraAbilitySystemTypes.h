#pragma once
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAsc = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAsc = nullptr;

	UPROPERTY()
	float BaseDamge = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const {return bIsBlockedHit;}
	void SetIsBlockedHit(const bool bIsInBlockedHit) {this->bIsBlockedHit = bIsInBlockedHit;}

	bool IsCritHit() const {return bIsCritHit;}
	void SetIsCritHit(const bool bIsInCritHit) {this->bIsCritHit = bIsInCritHit;}

	bool IsSuccessfulDebuff() const {return bIsSuccessfulDebuff;}
	void SetIsSuccessfulDebuff(const bool bIsInSuccessfulDebuff) {this->bIsSuccessfulDebuff = bIsInSuccessfulDebuff;}

	float GetDebuffDamage() const {return DebuffDamage;}
	void SetDebuffDamage(const float InDebuffDamage) {this->DebuffDamage = InDebuffDamage;}

	float GetDebuffFrequency() const {return DebuffFrequency;}
	void SetDebuffFrequency(const float InDebuffFrequency) {this->DebuffFrequency = InDebuffFrequency;}

	float GetDebuffDuration() const {return DebuffDuration;}
	void SetDebuffDuration(const float InDebuffDuration) {this->DebuffDuration = InDebuffDuration;}

	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) {this->DamageType = InDamageType;}
	TSharedPtr<FGameplayTag> GetDamageType() const {return DamageType;}
	
	virtual UScriptStruct* GetScriptStruct() const override {return StaticStruct();}
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCritHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	TSharedPtr<FGameplayTag> DamageType;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

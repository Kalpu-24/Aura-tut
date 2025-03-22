#pragma once
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAsc = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAsc = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamge = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector	DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForce = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector	KnockbackVector = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
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

	void SetDeathImpulse(const FVector& InDeathImpulse) {this->DeathImpulse = InDeathImpulse;}
	FVector GetDeathImpulse() const {return DeathImpulse;}

	void SetKnockbackVector(const FVector& InKnockbackVector) {this->KnockbackVector = InKnockbackVector;}
	FVector GetKnockbackVector() const {return KnockbackVector;}

	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	bool IsRadialDamage() const { return bIsRadialDamage; }

	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }

	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }

	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
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

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackVector = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;
 
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
 
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
 
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
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

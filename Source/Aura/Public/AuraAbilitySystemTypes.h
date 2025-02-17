#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetIsBlockedHit(const bool bIsInBlockedHit) {this->bIsBlockedHit = bIsInBlockedHit;}

	bool IsCritHit() const {return bIsCritHit;}

	void SetIsCritHit(const bool bIsInCritHit) {this->bIsCritHit = bIsInCritHit;}
	
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

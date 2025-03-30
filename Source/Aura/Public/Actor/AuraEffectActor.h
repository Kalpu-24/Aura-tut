// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	URotatingMovementComponent* RotatingMovementComponent;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect>& GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	bool bApplyEffectToEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TEnumAsByte<EEffectApplicationPolicy> InstantEffectApplicationPolicy = DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TEnumAsByte<EEffectApplicationPolicy> DurationEffectApplicationPolicy = DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TEnumAsByte<EEffectApplicationPolicy> InfiniteEffectApplicationPolicy = DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TEnumAsByte<EEffectRemovalPolicy> InfiniteEffectRemovalPolicy = RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Applied Effect")
	float ActorLevel = 1.f;
};

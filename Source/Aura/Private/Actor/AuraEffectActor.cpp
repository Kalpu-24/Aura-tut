// Copyright 2025 Kalp Games, All rights reserved.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor(){
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.f, 45.f, 0.f);
	RotatingMovementComponent->bRotationInLocalSpace = true;
	RotatingMovementComponent->bAutoActivate = true;
	RotatingMovementComponent->SetUpdatedComponent(GetRootComponent());
}

void AAuraEffectActor::BeginPlay(){
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect>& GameplayEffectClass){
	if (const bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy")); bIsEnemy && !bApplyEffectToEnemies) return;
	
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAsc == nullptr) return;

	UGameplayStatics::PlaySoundAtLocation(this, PickupSoundComponent, GetActorLocation());
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAsc->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite =  EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAsc);
	}

	if (!bIsInfinite && bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (const bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy")); bIsEnemy && !bApplyEffectToEnemies) return;
	if (InstantEffectApplicationPolicy == ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffect);
	}
	if (DurationEffectApplicationPolicy == ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffect);
	}
	if (InfiniteEffectApplicationPolicy == ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffect);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (const bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy")); bIsEnemy && !bApplyEffectToEnemies) return;
	if (InstantEffectApplicationPolicy == ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffect);
	}
	if (DurationEffectApplicationPolicy == ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffect);
	}
	if (InfiniteEffectApplicationPolicy == ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffect);
	}
	if (InfiniteEffectRemovalPolicy == RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetAsc)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetAsc == HandlePair.Value)
			{
				TargetAsc->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

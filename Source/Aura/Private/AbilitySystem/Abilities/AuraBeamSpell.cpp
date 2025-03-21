// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10,
				TraceTypeQuery1,
				false,
				IgnoreActors,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	if (ICombatInterface* CombatInterface =	Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeath().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeath().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTarget)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	TArray<AActor*> OverlapActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlapActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	const int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);

	UAuraAbilitySystemLibrary::GetClosestTargets(
		NumAdditionalTargets,
		OverlapActors,
		OutAdditionalTarget,
		MouseHitActor->GetActorLocation());

	for (const auto Target : OutAdditionalTarget)
	{
		if (ICombatInterface* CombatInterface =	Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeath().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalyTargetDied))
			{
				CombatInterface->GetOnDeath().AddDynamic(this, &UAuraBeamSpell::AdditionalyTargetDied);
			}
		}
	}
}

void UAuraBeamSpell::RemoveOnDeathBindingFromPrimaryTarget()
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		CombatInterface->GetOnDeath().RemoveDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
	}
}
 
void UAuraBeamSpell::RemoveOnDeathBindingFromAdditionalTarget(AActor* AdditionalTarget)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTarget))
	{
		CombatInterface->GetOnDeath().RemoveDynamic(this, &UAuraBeamSpell::AdditionalyTargetDied);
	}
}
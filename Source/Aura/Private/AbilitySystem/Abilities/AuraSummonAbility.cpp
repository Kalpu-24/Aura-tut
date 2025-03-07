// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"


TArray<FVector> UAuraSummonAbility::GetSpawnLocations() const
{
	const FVector ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / (NumMinions-1);

	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-SpawnSpread/2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenLocation + FVector(0.f,0.f,400.f), ChosenLocation - FVector(0.f,0.f,400.f),ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	if (MinionClass.Num() == 0) return nullptr;
	const int32 Selection = FMath::RandRange(0, MinionClass.Num()-1);
	return MinionClass[Selection];
}

AAuraCharacterBase* UAuraSummonAbility::GetRandomMinionClassCDO() const
{
	if (MinionClass.Num() == 0) return nullptr;
 
	const int32 RandomMinionClassIdx = FMath::RandRange(0, MinionClass.Num() - 1);
	return MinionClass[RandomMinionClassIdx]->GetDefaultObject<AAuraCharacterBase>();
}

// Copyright 2025 Kalp Games, All rights reserved.


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	Pt_0 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_0"));
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);

	Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_1"));
	ImmutablePts.Add(Pt_1);
	Pt_1->SetupAttachment(GetRootComponent());

	Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_2"));
	ImmutablePts.Add(Pt_2);
	Pt_2->SetupAttachment(GetRootComponent());

	Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_3"));
	ImmutablePts.Add(Pt_3);
	Pt_3->SetupAttachment(GetRootComponent());

	Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_4"));
	ImmutablePts.Add(Pt_4);
	Pt_4->SetupAttachment(GetRootComponent());

	Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_5"));
	ImmutablePts.Add(Pt_5);
	Pt_5->SetupAttachment(GetRootComponent());

	Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_6"));
	ImmutablePts.Add(Pt_6);
	Pt_6->SetupAttachment(GetRootComponent());

	Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_7"));
	ImmutablePts.Add(Pt_7);
	Pt_7->SetupAttachment(GetRootComponent());

	Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_8"));
	ImmutablePts.Add(Pt_8);
	Pt_8->SetupAttachment(GetRootComponent());

	Pt_9 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_9"));
	ImmutablePts.Add(Pt_9);
	Pt_9->SetupAttachment(GetRootComponent());

	Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_10"));
	ImmutablePts.Add(Pt_10);
	Pt_10->SetupAttachment(GetRootComponent());
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Not enough points in the collection!"));
	TArray<USceneComponent*> CopyPoints;

	for (USceneComponent* Pt : ImmutablePts)
	{
		if (CopyPoints.Num() >= NumPoints) return CopyPoints;
		if (Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500, GetActorLocation());

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), CollisionParams);

		const FVector AdjustedLoc = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLoc);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		CopyPoints.Add(Pt);
	}
	return CopyPoints;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

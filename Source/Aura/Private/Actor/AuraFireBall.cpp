// Copyright 2025 Kalp Games, All rights reserved.


#include "Actor/AuraFireBall.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

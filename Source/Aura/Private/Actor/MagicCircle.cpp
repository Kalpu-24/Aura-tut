// Copyright 2025 Kalp Games, All rights reserved.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"


AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MagicCircleDecal"));
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Copyright Kalp Shah

#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase(){
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay(){
	Super::BeginPlay();
}


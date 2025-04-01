// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "GameFramework/Actor.h"
#include "DemoBrawl.generated.h"

UCLASS()
class AURA_API ADemoBrawl : public AActor
{
	GENERATED_BODY()
	
public:
	ADemoBrawl();
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations() const;

	UFUNCTION(BlueprintPure, Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	UFUNCTION(BlueprintPure, Category="Summoning")
	AAuraCharacterBase* GetRandomMinionClassCDO() const;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MinSpawnDistance = 25.f;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MaxSpawnDistance = 450.f;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float SpawnSpread = 360.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Summoning")
	float Frequency = 10.f;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};

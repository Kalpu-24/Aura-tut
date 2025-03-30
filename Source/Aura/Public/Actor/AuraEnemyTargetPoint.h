// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemyTargetPoint.generated.h"

class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemyTargetPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
 
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<AAuraEnemy> EnemyClass;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 EnemyLevel = 1;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};

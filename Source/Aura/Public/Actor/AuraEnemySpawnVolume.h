// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AuraEnemyTargetPoint.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	

 	
public:	
	AAuraEnemySpawnVolume();
 
	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */
 
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
 
protected:
	virtual void BeginPlay() override;
 
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemyTargetPoint*> SpawnPoints;
private:
 
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

};

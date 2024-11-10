// Copyright Kalp Shah

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/TargetInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public ITargetInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	
	/** Target interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** End Target interface */

protected:
	virtual void BeginPlay() override;
};

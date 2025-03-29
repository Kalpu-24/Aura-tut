// Copyright 2025 Kalp Games, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/HighlightInterface.h"
#include "AuraPlayerController.generated.h"

class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor = nullptr;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh = nullptr;
  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded = false;
};

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};
class ITargetInterface;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController{
	
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	FHitResult GetCursorHit(){return CursorHit;}

	UFUNCTION(Client, Reliable, NetMulticast)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit);
	
	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
	
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/** How much of the Pawn capsule Radius and Height
  * should be used for the Line Trace before considering an Actor occluded?
  * Values too low may make the camera clip through walls.
  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion",
	  meta=(ClampMin="0.1", ClampMax="10.0") )
	float CapsulePercentageForTrace;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	UMaterialInterface* FadeMaterial;
 
	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	class USpringArmComponent* ActiveSpringArm;
 
	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	class UCameraComponent* ActiveCamera;
 
	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	class UCapsuleComponent* ActiveCapsuleComponent;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion")
	bool IsOcclusionEnabled;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool DebugLineTraces;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed() {bIsShiftKeyDown = true;}
	void ShiftReleased() {bIsShiftKeyDown = false;}
	bool bIsShiftKeyDown = false;

	void CursorTrace();

	UPROPERTY()
	TObjectPtr<AActor> LastActor;

	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UAuraAbilitySystemComponent* GetAsc();
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystrem;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation() const;
	
	UPROPERTY()
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;
  
	bool HideOccludedActor(const AActor* Actor);
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	static bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor);
	// ReSharper disable once CppFunctionIsNotImplemented
	void HandleCollision(TArray<FHitResult> OutHits);
	void ForceShowOccludedActors();
 
	__forceinline bool ShouldCheckCameraOcclusion() const
	{
		return IsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent;
	}
};

// Copyright 2025 Kalp Games, All rights reserved.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/TargetInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	CapsulePercentageForTrace = 1.0f;
	DebugLineTraces = true;
	IsOcclusionEnabled = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* TargetCharacter, const bool bIsBlockedHit, const bool bIsCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponent)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponent);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bIsBlockedHit, bIsCriticalHit);
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem) Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	if (IsValid(GetPawn()))
	{
		ActiveSpringArm = Cast<
		  USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
		ActiveCamera = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
		ActiveCapsuleComponent = Cast<UCapsuleComponent>(
		  GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetAsc() && GetAsc()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed)) return;
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetAsc() && GetAsc()->HasMatchingGameplayTag(TAG_Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
		LastActor = ThisActor = nullptr;
		return;
	}
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */
	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetAsc() && GetAsc()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed)) return;
	if (InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UTargetInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
			bAutoRunning = false;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
	}
	if (GetAsc()) GetAsc()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAsc() && GetAsc()->HasMatchingGameplayTag(TAG_Player_Block_InputReleased)) return;
	if (!InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		if (GetAsc()) GetAsc()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetAsc()) GetAsc()->AbilityInputTagReleased(InputTag);
	
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bIsShiftKeyDown)
	{
		if (const APawn* ControlledPawn = GetPawn(); FollowTime <= ShortPressThreshold && ControlledPawn)
		{

			if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
			{
				IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if (GetAsc() && !GetAsc()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystrem, CachedDestination);
			}
			if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavigationPath->PathPoints) Spline->AddSplinePoint(
					PointLoc, ESplineCoordinateSpace::World);
				if (!NavigationPath->PathPoints.IsEmpty())
				{
					CachedDestination = NavigationPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
		}
		TargetingStatus = ETargetingStatus::NotTargeting;
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetAsc() && GetAsc()->HasMatchingGameplayTag(TAG_Player_Block_InputHeld)) return;
	if (!InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		if (GetAsc()) GetAsc()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bIsShiftKeyDown)
	{
		if (GetAsc()) GetAsc()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector MoveDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(MoveDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAsc()
{
	if (AbilitySystemComponent == nullptr) AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	return AbilitySystemComponent;
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}
 
void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		if (const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length(); DistanceToDestination
			<= AutoRunAcceptanceRadius) bAutoRunning = false;
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation() const
{
	if (IsValid(MagicCircle))
	{
		if (CursorHit.bBlockingHit)
		{
			MagicCircle->SetActorHiddenInGame(false);
			MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
		}
		else
		{
			MagicCircle->SetActorHiddenInGame(true);
		}
	}
}

void AAuraPlayerController::SyncOccludedActors()
{
  if (!ShouldCheckCameraOcclusion()) return;
 
  // Camera is currently colliding, show all current occluded actors
  // and do not perform further occlusion
  if (ActiveSpringArm->bDoCollisionTest)
  {
    ForceShowOccludedActors();
    return;
  }
 
  FVector Start = ActiveCamera->GetComponentLocation();
  FVector End = GetPawn()->GetActorLocation();
 
  TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
  CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
 
  TArray<AActor*> ActorsToIgnore; 
  TArray<FHitResult> OutHits;
	
  auto ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
 
  bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
    GetWorld(), Start, End, ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
    ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace, CollisionObjectTypes, true,
    ActorsToIgnore,
    ShouldDebug,
    OutHits, true);

	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(ActorsToIgnore);

	bool bOverlap = GetWorld()->SweepMultiByChannel(
		Hits,
		ActiveCamera->GetComponentLocation(), // Start
		ActiveCamera->GetComponentLocation(), // End (same as start = overlap check)
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeSphere(50.f), // Adjust radius as needed
		Params
	);

	TArray<FHitResult> MergedHits;
	MergedHits.Append(OutHits);
	MergedHits.Append(Hits);
 
  if (bGotHits || bOverlap)
  {
    // The list of actors hit by the line trace, that means that they are occluded from view
    TSet<const AActor*> ActorsJustOccluded;
 
    // Hide actors that are occluded by the camera
    for (FHitResult Hit : MergedHits)
    {
      const AActor* HitActor = Cast<AActor>(Hit.GetActor());
      HideOccludedActor(HitActor);
      ActorsJustOccluded.Add(HitActor);
    }
 
    // Show actors that are currently hidden but that are not occluded by the camera anymore 
    for (auto& Elem : OccludedActors)
    {
      if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
      {
        ShowOccludedActor(Elem.Value);
 
        if (DebugLineTraces)
        {
          UE_LOG(LogTemp, Warning,
                 TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."), *Elem.Value.Actor->GetName());
        }
      }
    }
  }
  else
  {
    ForceShowOccludedActors();
  }
}
 
bool AAuraPlayerController::HideOccludedActor(const AActor* Actor)
{
  FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);
 
  if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
  {
    if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."),
                                *Actor->GetName());
    return false;
  }
 
  if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
  {
    ExistingOccludedActor->IsOccluded = true;
    OnHideOccludedActor(*ExistingOccludedActor);
 
    if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."), *Actor->GetName());
  }
  else
  {
    UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(
      Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
 
    FCameraOccludedActor OccludedActor;
    OccludedActor.Actor = Actor;
    OccludedActor.StaticMesh = StaticMesh;
    OccludedActor.Materials = StaticMesh->GetMaterials();
    OccludedActor.IsOccluded = true;
    OccludedActors.Add(Actor, OccludedActor);
    OnHideOccludedActor(OccludedActor);
 
    if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."), *Actor->GetName());
  }
 
  return true;
}
 
 
void AAuraPlayerController::ForceShowOccludedActors()
{
  for (auto& Elem : OccludedActors)
  {
    if (Elem.Value.IsOccluded)
    {
      ShowOccludedActor(Elem.Value);
 
      if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again."), *Elem.Value.Actor->GetName());
    }
  }
}
 
void AAuraPlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
  if (!IsValid(OccludedActor.Actor))
  {
    OccludedActors.Remove(OccludedActor.Actor);
  }
 
  OccludedActor.IsOccluded = false;
  OnShowOccludedActor(OccludedActor);
}
 
bool AAuraPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor)
{
  for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
  {
    OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
  }
	//set the mesh to block visibility and target channel
	  OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	  OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Block);
  return true;
}
 
bool AAuraPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
  for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
  {
    OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
  }

	//set the mesh to block visibility and target channel
	  OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	  OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
 
  return true;
}
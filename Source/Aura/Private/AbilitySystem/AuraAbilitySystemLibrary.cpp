// Copyright 2025 Kalp Games, All rights reserved.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemTypes.h"
#include "GameplayTagsManager.h"
#include "Game/AuraGameModeBase.h"
#include "HUD/AuraHUD.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWCParams(const UObject* WorldContext,  FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PlayerController = WorldContext->GetWorld()->GetFirstPlayerController())
	{
		OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			OutWCParams.AttributeSet = AttributeSet;
			OutWCParams.AbilitySystemComponent = AbilitySystemComponent;
			OutWCParams.PlayerController = PlayerController;
			OutWCParams.PlayerState = PlayerState;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams WCParam;
	if (AAuraHUD* AuraHUD = nullptr; MakeWCParams(WorldContext, WCParam, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParam);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams WCParam;
	if (AAuraHUD* AuraHUD = nullptr; MakeWCParams(WorldContext, WCParam, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParam);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams WCParam;
	if (AAuraHUD* AuraHUD = nullptr; MakeWCParams(WorldContext, WCParam, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParam);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContext, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* Asc)
{
	const AActor* AvatarActor = Asc->GetAvatarActor();
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext);
	if (ClassInfo == nullptr) return;
	
	const FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDeafultInfo(CharacterClass);
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = Asc->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryEffectSpecHandle = Asc->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle SecondaryEffectSpecHandle = Asc->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, PrimaryAttributeContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle VitalEffectSpecHandle = Asc->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, PrimaryAttributeContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldObject, UAbilitySystemComponent* Asc, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldObject);
	if (ClassInfo == nullptr) return;
	
	for (const TSubclassOf CommonAbility : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		Asc->GiveAbility(AbilitySpec);
	}
	for (const FCharacterClassDefaultInfo& CharacterClassDefaultInfo = ClassInfo->GetClassDeafultInfo(CharacterClass); TSubclassOf AbilityClass : CharacterClassDefaultInfo.StartupAbilities)
	{
		int32 PlayerLevel = 1;
		if (Asc->GetAvatarActor()->Implements<UCombatInterface>())
		{
			PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Asc->GetAvatarActor());
		}
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, PlayerLevel);
		Asc->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldObject)
{
	const AAuraGameModeBase* AuraGmb = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));
	if (AuraGmb == nullptr) return nullptr;
	return AuraGmb->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldObject)
{
	const AAuraGameModeBase* AuraGmb = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));
	if (AuraGmb == nullptr) return nullptr;
	return AuraGmb->AbilityInfo;
}


int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContext,
                                                             ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext);
	if (ClassInfo == nullptr) return 0;
	return static_cast<int32>(ClassInfo->GetClassDeafultInfo(CharacterClass).XPReward.GetValueAtLevel(CharacterLevel));
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCritHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsInBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bIsInBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsInCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCritHit(bIsInCriticalHit);
	}
}

TMap<FGameplayTag, FGameplayTag> UAuraAbilitySystemLibrary::GetDamageToResistanceTags()
{
	TMap<FGameplayTag, FGameplayTag> OutMap;
	// Get the parent tag 
	const FGameplayTag DamageTypeParentTag = FGameplayTag::RequestGameplayTag(FName("DamageType"));
	const FGameplayTag ResistanceTypeParentTag = FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance"));
    
	// Get the tag node from the hierarchy
	TArray<TSharedPtr<FGameplayTagNode>> DamageTypeChildTagNodes = UGameplayTagsManager::Get().FindTagNode(DamageTypeParentTag)->GetChildTagNodes();
	TArray<TSharedPtr<FGameplayTagNode>> ResistanceTypeChildTagNodes = UGameplayTagsManager::Get().FindTagNode(ResistanceTypeParentTag)->GetChildTagNodes();

	for (int i = 0; i < DamageTypeChildTagNodes.Num(); i++)
	{
		OutMap.Add(DamageTypeChildTagNodes[i]->GetCompleteTag(), ResistanceTypeChildTagNodes[i]->GetCompleteTag());
	}
	return OutMap;
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContext,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius,
	const FVector& SphereLocation)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool bSecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	return bFirstIsPlayer != bSecondIsPlayer;
}

TMap<FGameplayTag, FGameplayTag> UAuraAbilitySystemLibrary::GetDamageTypesToDebuffTags()
{
	TMap<FGameplayTag, FGameplayTag> OutMap;
	// Get the parent tag
	const FGameplayTag DamageTypeParentTag = FGameplayTag::RequestGameplayTag(FName("DamageType"));
	const FGameplayTag DebuffTypeParentTag = FGameplayTag::RequestGameplayTag(FName("Debuff"));

	// Get the tag node from the hierarchy
	TArray<TSharedPtr<FGameplayTagNode>> DamageTypeChildTagNodes = UGameplayTagsManager::Get().FindTagNode(DamageTypeParentTag)->GetChildTagNodes();
	TArray<TSharedPtr<FGameplayTagNode>> DebuffTypeChildTagNodes = UGameplayTagsManager::Get().FindTagNode(DebuffTypeParentTag)->GetChildTagNodes();

	for (int i = 0; i < DamageTypeChildTagNodes.Num(); i++)
	{
		OutMap.Add(DamageTypeChildTagNodes[i]->GetCompleteTag(), DebuffTypeChildTagNodes[i]->GetCompleteTag());
	}

	return OutMap;
}

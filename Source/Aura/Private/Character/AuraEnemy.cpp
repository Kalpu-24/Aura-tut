// Copyright 2025 Kalp Games, All rights reserved.


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy(){
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor(){
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor(){
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay(){
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UAuraUserWidget* HealthBarWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		HealthBarWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAs = CastChecked<UAuraAttributeSet>(AttributeSet)){
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAs->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){
				OnHealthChange.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAs->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){
				OnMaxHealthChange.Broadcast(Data.NewValue);
			}
		);

		OnHealthChange.Broadcast(AuraAs->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAs->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

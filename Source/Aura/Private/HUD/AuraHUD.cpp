// Copyright Kalp Shah


#include "HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"

void AAuraHUD::BeginPlay(){
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}

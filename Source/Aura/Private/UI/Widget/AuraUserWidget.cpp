// Copyright 2025 Kalp Games, All rights reserved.


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController){
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

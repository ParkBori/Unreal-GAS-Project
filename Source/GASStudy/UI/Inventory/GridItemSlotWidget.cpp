// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/GridItemSlotWidget.h"

void UGridItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetRenderOpacity(0.5f);
}


void UGridItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetRenderOpacity(1.f);

}

void UGridItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetRenderOpacity(0.5f);
}

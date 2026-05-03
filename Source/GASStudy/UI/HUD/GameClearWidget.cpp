// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/GameClearWidget.h"

UGameClearWidget::UGameClearWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mWidgetName = TEXT("GameClearWidget");

	if (IsValid(ActivateImg))
	{
		FWidgetAnimationDynamicEvent WidgetAnimationDynamicEvent;
		WidgetAnimationDynamicEvent.BindUFunction(this, TEXT("HiddenGameClearWidget"));
		this->BindToAnimationFinished(ActivateImg, WidgetAnimationDynamicEvent);
	}
}

void UGameClearWidget::ShowGameCleareWidget()
{
	if (IsValid(ActivateImg))
	{
		PlayAnimation(ActivateImg, 0.f, 1, EUMGSequencePlayMode::Type::Forward);
	}
}

void UGameClearWidget::HiddenGameClearWidget()
{
	//SetVisibility(ESlateVisibility::Collapsed);
}

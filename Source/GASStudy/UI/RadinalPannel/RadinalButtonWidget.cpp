// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RadinalPannel/RadinalButtonWidget.h"

void URadinalButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Trigger))
	{
		Btn_Trigger->OnClicked.AddDynamic(this, &URadinalButtonWidget::OnClickedCallback_Btn_Trigger);
		Btn_Trigger->OnHovered.AddDynamic(this, &URadinalButtonWidget::OnHoveredCallback_Btn_Trigger);
		Btn_Trigger->OnUnhovered.AddDynamic(this, &URadinalButtonWidget::OnUnHoveredCallback_Btn_Trigger);
	}
}

void URadinalButtonWidget::OnClickedCallback_Btn_Trigger()
{
	if (mOnClickButton.IsBound())
	{
		mOnClickButton.Broadcast();
	}

	if (mOnActivateAbility.IsBound())
	{
		mOnActivateAbility.Broadcast(mAbilityTag);
	}
}

void URadinalButtonWidget::OnHoveredCallback_Btn_Trigger()
{
	mOnHoveredButton.Broadcast();
}

void URadinalButtonWidget::OnUnHoveredCallback_Btn_Trigger()
{
	mOnUnHoveredButton.Broadcast();
}

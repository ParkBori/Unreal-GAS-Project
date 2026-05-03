// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CheckTextWidget.h"


void UCheckTextWidget::SetCheckBox(bool bCheck)
{
	CB_IsComplete->SetCheckedState(bCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UCheckTextWidget::SetText(const FText& Desc)
{
	TB_Description->SetText(Desc);
}
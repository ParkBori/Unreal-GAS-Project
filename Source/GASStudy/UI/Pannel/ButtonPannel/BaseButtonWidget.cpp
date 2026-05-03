// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"

void UBaseButtonWidget::NativePreConstruct()
{
	if (IsValid(CT_BtnName))
	{
		CT_BtnName->SetStyle(mTextStyle.Noraml.TextStyle);
		CT_BtnName->SetText(mButtonNameText);
	}
}

void UBaseButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnHovered().AddUObject(this, &UBaseButtonWidget::OnHoveredCallback);
	OnUnhovered().AddUObject(this, &UBaseButtonWidget::OnUnHoveredCallback);
}

void UBaseButtonWidget::SetBtnName(FText BtnName)
{
	if (IsValid(CT_BtnName))
	{
		CT_BtnName->SetText(BtnName);
		mButtonNameText = BtnName;
	}
}

void UBaseButtonWidget::OnHoveredCallback()
{
	ChangeStyle(true);
}

void UBaseButtonWidget::OnUnHoveredCallback()
{
	ChangeStyle(false);
}

void UBaseButtonWidget::ChangeStyle(bool bHovered)
{
	// 스타일 바꿔주기

	TSubclassOf<UCommonTextStyle> TextStyle;

	if (bHovered)
	{
		if (IsValid(mTextStyle.Hover.TextStyle))
		{
			TextStyle = mTextStyle.Hover.TextStyle;
		}
	}
	else
	{
		if (IsValid(mTextStyle.Noraml.TextStyle))
		{
			TextStyle = mTextStyle.Noraml.TextStyle;
		}
	}

	if (IsValid(TextStyle))
	{
		CT_BtnName->SetStyle(TextStyle);
	}
}
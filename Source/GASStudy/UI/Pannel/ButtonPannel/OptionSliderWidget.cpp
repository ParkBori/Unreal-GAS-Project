// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ButtonPannel/OptionSliderWidget.h"


void UOptionSliderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetSliderInfo(mSliderCurrentValue, mSliderMinValue, mSliderMaxValue, mSliderStepSize);

	bCanBroadcastOnOptionChanged = true;

	if (IsValid(SL_Option))
	{
		SL_Option->OnValueChanged.AddDynamic(this, &UOptionSliderWidget::OnValueChangedCallback_SL_Option);
	}

	if (IsValid(CT_OptionName))
	{
		CT_OptionName->SetText(mSetting.ButtonName);
	}


	if (IsValid(SL_Option))
	{
		SL_Option->SetSliderBarColor(mSliderNormalColor);
		SL_Option->SetSliderHandleColor(mSliderNormalColor);
	}

	OnHovered().AddUObject(this, &UOptionSliderWidget::OnHoveredCallback);
	OnUnhovered().AddUObject(this, &UOptionSliderWidget::OnUnHoveredCallback);
}

void UOptionSliderWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOptionSliderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(CT_OptionName))
	{
		CT_OptionName->SetText(mSetting.ButtonName);
	}
}

void UOptionSliderWidget::OnValueChangedCallback_SL_Option(float Value)
{
	if (!bCanBroadcastOnOptionChanged)
	{
		return;
	}

	float FloorValue = int32(Value * 100) / 100.f;

 	if (IsValid(CT_Value))
	{
		int32 IValue = FloorValue * 100.f;
		CT_Value->SetText(FText::AsNumber((int32)Value));
	}

	mOnOptionChanged.Broadcast(Value);
}

void UOptionSliderWidget::OnHoveredCallback()
{
	ChangeStyle(true);
}

void UOptionSliderWidget::OnUnHoveredCallback()
{
	ChangeStyle(false);
}

void UOptionSliderWidget::ChangeStyle(bool bHovered)
{
	// 스타일 바꿔주기

	TSubclassOf<UCommonTextStyle> TextStyle;
	FLinearColor ChangedColor;
	if (bHovered)
	{
		if (IsValid(mTextStyle.Hover.TextStyle))
		{
			TextStyle = mTextStyle.Hover.TextStyle;
		}
		ChangedColor = mSliderHoverColor;
	}
	else
	{
		if (IsValid(mTextStyle.Noraml.TextStyle))
		{
			TextStyle = mTextStyle.Noraml.TextStyle;
		}
		ChangedColor = mSliderNormalColor;
	}

	if (IsValid(TextStyle))
	{
		CT_OptionName->SetStyle(TextStyle);
		CT_Value->SetStyle(TextStyle);
	}

	if (IsValid(SL_Option))
	{
		SL_Option->SetSliderBarColor(ChangedColor);
		SL_Option->SetSliderHandleColor(ChangedColor);
	}

}

void UOptionSliderWidget::ChangeStateValue(float NewStateValue)
{
	bCanBroadcastOnOptionChanged = false;

	if (IsValid(CT_Value))
	{
		CT_Value->SetText(FText::AsNumber((int32)NewStateValue));
	}

	if (IsValid(SL_Option))
	{
		SL_Option->SetValue((int32)NewStateValue);
	}

	bCanBroadcastOnOptionChanged = true;
}

void UOptionSliderWidget::SetSliderInfo(float CurrentValue, float MinValue, float MaxValue, float StepSize)
{
	bCanBroadcastOnOptionChanged = false;

	if (IsValid(CT_Value))
	{
		//SL_Option->SetValue(int32(CurrentValue));
		SL_Option->SetMinValue(int32(MinValue));
		SL_Option->SetMaxValue(int32(MaxValue));
		SL_Option->SetStepSize(StepSize);
	}

	if (IsValid(CT_Value))
	{
		CT_Value->SetText(FText::AsNumber(int32(CurrentValue)));
	}

	bCanBroadcastOnOptionChanged = true;
}

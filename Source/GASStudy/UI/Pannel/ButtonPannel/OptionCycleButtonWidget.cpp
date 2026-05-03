// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ButtonPannel/OptionCycleButtonWidget.h"
#include "OptionSliderWidget.h"

void UOptionCycleButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(CT_OptionName))
	{
		CT_OptionName->SetText(mSetting.ButtonName);
	}
}

void UOptionCycleButtonWidget::NativeConstruct()
{
	if (IsValid(Btn_Prev))
	{
		Btn_Prev->OnClicked.AddDynamic(this, &UOptionCycleButtonWidget::OnClickedCallback_CB_Prev);
	}

	if (IsValid(Btn_Next))
	{
		Btn_Next->OnClicked.AddDynamic(this, &UOptionCycleButtonWidget::OnClickedCallback_CB_Next);
	}

	OnHovered().AddUObject(this, &UOptionCycleButtonWidget::OnHoveredCallback);
	OnUnhovered().AddUObject(this, &UOptionCycleButtonWidget::OnUnHoveredCallback);
}

void UOptionCycleButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UOptionCycleButtonWidget::OnHoveredCallback()
{
	ChangeStyle(true);
}

void UOptionCycleButtonWidget::OnUnHoveredCallback()
{
	ChangeStyle(false);
}

void UOptionCycleButtonWidget::OnClickedCallback_CB_Prev()
{
	UE_LOG(LogTemp, Display, TEXT("Clicked"));
	ChangeStateValue(false);
	mOnOptionChanged.Broadcast(mCurrentStateValue);
}

void UOptionCycleButtonWidget::OnClickedCallback_CB_Next()
{
	UE_LOG(LogTemp, Display, TEXT("Clicked"));
	ChangeStateValue(true);
	mOnOptionChanged.Broadcast(mCurrentStateValue);
}

void UOptionCycleButtonWidget::ChangeStyle(bool bHovered)
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
		CT_OptionName->SetStyle(TextStyle);
		CT_OptionValue->SetStyle(TextStyle);
		CT_OptionValueLeftText->SetStyle(TextStyle);
		CT_OptionValueRightText->SetStyle(TextStyle);
	}
}

void UOptionCycleButtonWidget::ChangeStateValue(bool bNext)
{
	int StateCount = mSetting.StateValues.Num();
	if (StateCount > 0)
	{
		if (bNext)
		{
			mCurrentStateValue++;
			mCurrentStateValue = mCurrentStateValue % StateCount;
		}
		else
		{
			mCurrentStateValue--;

			if (mCurrentStateValue < 0)
			{
				mCurrentStateValue = StateCount - 1;
			}
		}
		CT_OptionValue->SetText(mSetting.StateValues[mCurrentStateValue]);
	}
}

void UOptionCycleButtonWidget::ChangeStateValue(int32 NewStateValue)
{
	if (mSetting.StateValues.IsValidIndex(mCurrentStateValue))
	{
		// 여기에 값이 안들어오네.. 왜지? test 가 비어있음
		if (mSetting.StateValues.IsValidIndex(NewStateValue))
		{
			const auto& StateText = mSetting.StateValues[NewStateValue];
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, *FString::FromInt(NewStateValue));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, *StateText.ToString());
			CT_OptionValue->SetText(StateText);
			mCurrentStateValue = NewStateValue;
		}
	}
}


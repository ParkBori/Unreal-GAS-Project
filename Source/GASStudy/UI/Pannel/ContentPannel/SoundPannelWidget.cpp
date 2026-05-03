// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ContentPannel/SoundPannelWidget.h"
#include "UI/ToolTip/DefaultToolTipWidget.h"
#include "UI/Pannel/ButtonPannel/OptionSliderWidget.h"
#include "Subsystem/PopupManageSubsystem.h"
#include "Subsystem/UserSettingSubsystem.h"

void USoundPannelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	TArray<TObjectPtr<UOptionSliderWidget>> ChildSlider;
	for (auto& W : SCB_Options->GetAllChildren())
	{
		UOptionSliderWidget* Slider = Cast<UOptionSliderWidget>(W);
		if (IsValid(Slider))
		{
			ChildSlider.Add(Slider);
		}
	}

	BindOnHoverded_Slider(ChildSlider);
	BindOnUnHoverded_Slider(ChildSlider);


	//Sl_MasterVolume->mOnOptionChanged.AddDynamic(this, &USoundPannelWidget::Callback_MasterVolumeChanged);;

	Sl_BGMVolume->mOnOptionChanged.AddDynamic(this, &USoundPannelWidget::Callback_BGMVolumeChanged);;

	Sl_SFXVolume->mOnOptionChanged.AddDynamic(this, &USoundPannelWidget::Callback_SFXVolumeChanged);;

	Sl_UIVolume->mOnOptionChanged.AddDynamic(this, &USoundPannelWidget::Callback_UIVolumeChanged);;


	SetSavedValue();
}

bool USoundPannelWidget::CheckIsAnySettingDirty()
{
	bool bChanged = !(mCurrentSoundData == mChangedSoundData);

	if (bChanged)
	{
		bHasChanges = true;
	}
	else
	{
		bHasChanges = false;
	}

	return bHasChanges;
}

void USoundPannelWidget::Back()
{
	if (CheckIsAnySettingDirty())
	{
		UPopupManageSubsystem* PopupManageSubsystem = GetGameInstance()->GetSubsystem<UPopupManageSubsystem>();
		if (IsValid(PopupManageSubsystem))
		{
			FPopupInfo PopupInfo;
			PopupInfo.Title = FText::FromString(TEXT("Dont Save UserInfo"));
			PopupInfo.Script = FText::FromString(TEXT("Want save Click confirm. Isnt Click cancle"));
			PopupInfo.bOnlyConfirm = false;
			PopupInfo.bUserTimer = true;
			PopupInfo.OnConfirmDelegate.AddDynamic(this, &USoundPannelWidget::Callback_ConfirmApplyChange);
			PopupInfo.OnCancleDelegate.AddDynamic(this, &USoundPannelWidget::Callback_CancleApplyChange);

			PopupManageSubsystem->CreatePopup(PopupInfo);
		}
	}
	// 변경사항이 존재하지 않으면, 설정 위젯을 제거한다.
	else
	{
		RemoveFromParent();
	}
}

void USoundPannelWidget::ApplyChange()
{
	if (!CheckIsAnySettingDirty())
	{
		return;
	}

	// 변경 사항이 존재한다면 세이브 
	auto* PC = GetOwningPlayer();
	if (!IsValid(PC))
	{
		return;
	}

	auto* USS = PC->GetGameInstance()->GetSubsystem<UUserSettingSubsystem>();
	if (!IsValid(USS))
	{
		return;
	}

	mCurrentSoundData = mChangedSoundData;
	USS->SetSounds(mCurrentSoundData);

	USS->SaveSound();
}

void USoundPannelWidget::Reset()
{
	// 기본 값으로 설정 
	auto* PC = GetOwningPlayer();
	if (!IsValid(PC))
	{
		return;
	}

	auto* USS = PC->GetGameInstance()->GetSubsystem<UUserSettingSubsystem>();
	if (!IsValid(USS))
	{
		return;
	}

	USS->SetSoundsDefault(mCurrentSoundData);
	mChangedSoundData = mCurrentSoundData;
	USS->SaveSound();
	UpdateUI(mCurrentSoundData);
}

void USoundPannelWidget::Confirm()
{
}

void USoundPannelWidget::Revert()
{

}

void USoundPannelWidget::UpdateUI(const FSoundSaveData& SaveData)
{
	//Sl_MasterVolume->ChangeStateValue(SaveData.Master);
	Sl_BGMVolume->ChangeStateValue(SaveData.BGM);
	Sl_SFXVolume->ChangeStateValue(SaveData.SFX);
	Sl_UIVolume->ChangeStateValue(SaveData.UI);
}

void USoundPannelWidget::SetSavedValue()
{
	auto* PC = GetOwningPlayer();
	if (!IsValid(PC))
	{
		return;
	}

	auto* USS = PC->GetGameInstance()->GetSubsystem<UUserSettingSubsystem>();
	if (!IsValid(USS))
	{
		return;
	}

	FSoundSaveData Data = USS->GetSavedSoundData();
	mChangedSoundData = mCurrentSoundData = Data;
	USS->SetSounds(mCurrentSoundData);
	UpdateUI(mCurrentSoundData);
}

void USoundPannelWidget::BindOnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr)
{
	for (auto& Slider : SliderArr)
	{
		Slider->OnHovered().AddLambda([this, Slider]() {
			OnHoveredCallback_Slider(Slider);
			});
	}
}

void USoundPannelWidget::BindOnUnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr)
{
	for (auto& Slider : SliderArr)
	{
		Slider->OnUnhovered().AddLambda([this, Slider]() {
			OnUnHoveredCallback_Slider(Slider);
			});
	}
}

void USoundPannelWidget::OnHoveredCallback_Slider(UOptionSliderWidget* Slider)
{
	if (IsValid(Tooltip_OptionDesc))
	{
		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
		Tooltip_OptionDesc->SetDescript(Slider->mSetting.ButtonDescript);
	}
}

void USoundPannelWidget::OnUnHoveredCallback_Slider(UOptionSliderWidget* Slider)
{
	if (IsValid(Tooltip_OptionDesc))
	{
		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::Hidden);
	}
}

//void USoundPannelWidget::Callback_MasterVolumeChanged(float StateValue)
//{
//	mChangedSoundData.Master = StateValue;
//}

void USoundPannelWidget::Callback_BGMVolumeChanged(float StateValue)
{
	mChangedSoundData.BGM = StateValue;
}

void USoundPannelWidget::Callback_SFXVolumeChanged(float StateValue)
{
	mChangedSoundData.SFX = StateValue;
}

void USoundPannelWidget::Callback_UIVolumeChanged(float StateValue)
{
	mChangedSoundData.UI = StateValue;
}

void USoundPannelWidget::Callback_ConfirmApplyChange()
{
	ApplyChange();
	RemoveFromParent();
}

void USoundPannelWidget::Callback_CancleApplyChange()
{
	if (bHasChanges)
	{
		return;
	}

	// 변경 사항이 존재한다면 세이브 
	auto* PC = GetOwningPlayer();
	if (!IsValid(PC))
	{
		return;
	}

	auto* USS = PC->GetGameInstance()->GetSubsystem<UUserSettingSubsystem>();
	if (!IsValid(USS))
	{
		return;
	}

	USS->SetSounds(mCurrentSoundData);
	mChangedSoundData = mCurrentSoundData;
	USS->SaveSound();
	RemoveFromParent();
}
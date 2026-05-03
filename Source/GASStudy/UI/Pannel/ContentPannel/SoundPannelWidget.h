// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Pannel/ContentPannel/SettingContentPannelWidget.h"
#include "Save/Sound/Data/SoundSaveData.h"
#include "SoundPannelWidget.generated.h"


class UOptionSliderWidget;

/**
 *
 */
UCLASS()
class GASSTUDY_API USoundPannelWidget : public USettingContentPannelWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual bool CheckIsAnySettingDirty() override;

	virtual void Back() override;
	virtual void ApplyChange() override;
	virtual void Reset() override;

	// 사용 x 
	virtual void Confirm() override;
	virtual void Revert() override;

private:
	void UpdateUI(const FSoundSaveData& SaveData);

	void SetSavedValue();

private:
	void BindOnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr);
	void BindOnUnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr);

	void OnHoveredCallback_Slider(UOptionSliderWidget* Slider);
	void OnUnHoveredCallback_Slider(UOptionSliderWidget* Slider);

private:
	//UFUNCTION()
	//void Callback_MasterVolumeChanged(float StateValue);

	UFUNCTION()
	void Callback_BGMVolumeChanged(float StateValue);

	UFUNCTION()
	void Callback_SFXVolumeChanged(float StateValue);

	UFUNCTION()
	void Callback_UIVolumeChanged(float StateValue);

	UFUNCTION()
	void Callback_ConfirmApplyChange();

	UFUNCTION()
	void Callback_CancleApplyChange();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SCB_Options;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UOptionSliderWidget> Sl_MasterVolume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionSliderWidget> Sl_BGMVolume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionSliderWidget> Sl_SFXVolume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionSliderWidget> Sl_UIVolume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDefaultToolTipWidget> Tooltip_OptionDesc;

private:
	FSoundSaveData mCurrentSoundData;
	FSoundSaveData mChangedSoundData;
};

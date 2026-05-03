// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "UI/UIInfo.h"
#include "OptionSliderWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UOptionSliderWidget : public UCommonButtonBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnValueChangedCallback_SL_Option(float Value);

private:
	void OnHoveredCallback();
	void OnUnHoveredCallback();

private:
	void ChangeStyle(bool bHovered);

private:
	void ChangeStateValue(bool bNext);

public:
	void ChangeStateValue(float NewStateValue);
	void SetSliderInfo(float CurrentValue, float MinValue, float MaxValue, float StepSize);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_OptionName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_Value;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SL_Option;

public:
	UPROPERTY(EditAnywhere, Category = "Setting")
	FSettings_Slider mSetting;

	UPROPERTY(EditAnywhere, Category = "Setting")
	FButtonTextStyle mTextStyle;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float mSliderCurrentValue;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float mSliderMinValue;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float mSliderMaxValue;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float mSliderStepSize;

	UPROPERTY(EditAnywhere, Category = "Setting")
	FLinearColor mSliderNormalColor;
	UPROPERTY(EditAnywhere, Category = "Setting")
	FLinearColor mSliderHoverColor;

	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 mCurrentStateValue;

	bool bCanBroadcastOnOptionChanged = false;

public:
	UPROPERTY(BlueprintAssignable)
	FOnOptionChanged_float mOnOptionChanged;
};

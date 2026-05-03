// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIInfo.h"
#include "CommonButtonBase.h"
#include "OptionCycleButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UOptionCycleButtonWidget : public UCommonButtonBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;


private:
	void OnHoveredCallback();
	void OnUnHoveredCallback();

	UFUNCTION()
	void OnClickedCallback_CB_Prev();
	UFUNCTION()
	void OnClickedCallback_CB_Next();

	void ChangeStyle(bool bHovered);

private:
	void ChangeStateValue(bool bNext);

public:
	void ChangeStateValue(int32 NewStateValue);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Prev;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Next;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_OptionName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_OptionValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_OptionValueLeftText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_OptionValueRightText;

public:
	UPROPERTY(EditAnywhere, Category = "Setting")
	FSettings mSetting;

	UPROPERTY(EditAnywhere, Category = "Setting")
	FButtonTextStyle mTextStyle;

	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 mCurrentStateValue = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FOnOptionChanged mOnOptionChanged;
};

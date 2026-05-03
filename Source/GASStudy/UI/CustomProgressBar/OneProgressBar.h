// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "OneProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UOneProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(float CurrentValue, float MaxValue);

	void ChangedTargetValue(const struct FOnAttributeChangeData& ChangedData);
	void ChangedMaxValue(const struct FOnAttributeChangeData& ChangedData);

private:
	void Increase();

	void ClearTimer();


private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> FrontProgressBar;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> mIncreaseCurve;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float mCurrentHp;
	UPROPERTY(BlueprintReadWrite, /*FieldNotify,*/ meta = (AllowPrivateAccess))
	float mTargetValue;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float mMaxHp;

	FTimerHandle mProgressBarUpdateTimerHandle;

	float mAccDelta = 0.0f;
};

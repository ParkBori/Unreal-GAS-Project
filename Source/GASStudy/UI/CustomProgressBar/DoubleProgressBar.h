// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "DoubleProgressBar.generated.h"


/**
 * 이중 게이지 (증감량 표시)
 */
UCLASS()
class GASSTUDY_API UDoubleProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(float CurrentValue, float MaxValue);
	
	
	void ChangedTargetValue(const FOnAttributeChangeData& ChangedData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);


private:
	void Increase();
	void Decrease();

	void ClearTimer();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> FrontProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> BackProgressBar;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> mIncreaseCurve;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> mDecreaseCurve;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<USlider>PS_Percent;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float mCurrentHp;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float mTargetValue;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float mMaxHp;


	FTimerHandle mProgressBarUpdateDelay;
	FTimerHandle mProgressBarUpdateTimerHandle;

	float mAccDelta = 0.0f;
};

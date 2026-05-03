// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomProgressBar/OneProgressBar.h"
#include "GameplayEffectTypes.h"

void UOneProgressBar::Init(float CurrentValue, float MaxValue)
{
	mCurrentHp = CurrentValue;
	mMaxHp = MaxValue;

	float Percent = mCurrentHp / mMaxHp;
	FrontProgressBar->SetPercent(Percent);
}

void UOneProgressBar::ChangedTargetValue(const FOnAttributeChangeData& ChangedData)
{
	GEngine->AddOnScreenDebugMessage(3, 1.1f, FColor::Red, *FString::SanitizeFloat(ChangedData.NewValue));
	FrontProgressBar->SetPercent(ChangedData.NewValue / mMaxHp);
}

void UOneProgressBar::ChangedMaxValue(const FOnAttributeChangeData& ChangedData)
{
	mMaxHp = ChangedData.NewValue;

	float Percent = mCurrentHp / mMaxHp;
	FrontProgressBar->SetPercent(Percent);
}

void UOneProgressBar::Increase()
{
	if (false == IsValid(mIncreaseCurve))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (false == IsValid(World))
	{
		return;
	}

	// 커브데이터 시작시간 끝시간 얻기
	float Start, End;
	mIncreaseCurve->GetTimeRange(Start, End);

	// 델타 타임 누적
	mAccDelta += World->GetDeltaSeconds();
	// 델타타임 누적으로 알파값 얻기

	mAccDelta = FMath::Clamp<float>(mAccDelta, Start, End);
	float Alpha = mIncreaseCurve->GetFloatValue(mAccDelta);

	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, *FString::SanitizeFloat(Alpha));

	// 누적시간을 바탕으로 현재 값 Currnet 값 만드릭
	mCurrentHp = FMath::Lerp<float>(mCurrentHp, mTargetValue, Alpha);
	FrontProgressBar->SetPercent(mCurrentHp / mMaxHp);

	if (mAccDelta >= End)
	{
		mAccDelta = 0;
		ClearTimer();
	}
	else
	{
		World->GetTimerManager().SetTimer(mProgressBarUpdateTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				Increase();
			}), 0.01, false);
	}
}

void UOneProgressBar::ClearTimer()
{
	UWorld* World = GetWorld();
	if (false == IsValid(World))
	{
		return;
	}

	World->GetTimerManager().ClearTimer(mProgressBarUpdateTimerHandle);
	mProgressBarUpdateTimerHandle.Invalidate();
}


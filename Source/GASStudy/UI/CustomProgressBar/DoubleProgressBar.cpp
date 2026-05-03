// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleProgressBar.h"
#include "GameplayEffectTypes.h"

#include "Shared/Macro/Log.h"

void UDoubleProgressBar::Init(float CurrentValue, float MaxValue)
{
	mTargetValue = mCurrentHp = CurrentValue;
	mMaxHp = MaxValue;

	float Percent = mCurrentHp / mMaxHp;
	PS_Percent->SetValue(Percent);
	FrontProgressBar->SetPercent(Percent);
	BackProgressBar->SetPercent(Percent);
}

void UDoubleProgressBar::ChangedTargetValue(const FOnAttributeChangeData& ChangedData)
{
	ClearTimer();
	mAccDelta = 0.0f;

	// 체력이 증가되었는지 감소되었는지 판단


	float Diff = ChangedData.NewValue - mTargetValue;
	MY_LOG(Error, "NewValue:%f / OldValue:%f / TargetValue:%f / Diff:%f", ChangedData.NewValue, ChangedData.OldValue, mTargetValue, Diff)

	float TempPrevTargetValue = mTargetValue;
	mTargetValue = ChangedData.NewValue;
	// mMaxHp = MaxValue;

	if (Diff > 0.0f)
	{
		// 체력 증가 
		mCurrentHp = TempPrevTargetValue;
		float Percent = mCurrentHp / mMaxHp;
		BackProgressBar->SetPercent(Percent);
		Increase();
	}
	else if (Diff == 0.0f)
	{
		return;
	}

	else
	{
		// 체력 감소 
		float Percent = mTargetValue / mMaxHp;
		FrontProgressBar->SetPercent(Percent);
		PS_Percent->SetValue(Percent);

		GetWorld()->GetTimerManager().SetTimer(mProgressBarUpdateDelay, this, &UDoubleProgressBar::Decrease, 0.3, false);
	}
}

void UDoubleProgressBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	mMaxHp = Data.NewValue;
	GetWorld()->GetTimerManager().ClearTimer(mProgressBarUpdateDelay);

	float Percent = mTargetValue / mMaxHp;
	BackProgressBar->SetPercent(Percent);
	FrontProgressBar->SetPercent(Percent);
	PS_Percent->SetValue(Percent);

}

void UDoubleProgressBar::Increase()
{
	if (!IsValid(mIncreaseCurve))
	{
		return;
	}

	auto* OwningPlayer = GetOwningPlayer();
	UWorld* World = nullptr;
	if (!IsValid(OwningPlayer))
	{
		return;
	}

	if (!IsValid(World))
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

	// 누적시간을 바탕으로 현재 값 Currnet 값 만드릭
	mCurrentHp = FMath::Lerp<float>(mCurrentHp, mTargetValue, Alpha);
	
	float Percent = mCurrentHp / mMaxHp;
	BackProgressBar->SetPercent(Percent);
	FrontProgressBar->SetPercent(Percent);
	PS_Percent->SetValue(Percent);

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

void UDoubleProgressBar::Decrease()
{
	if (!IsValid(mDecreaseCurve))
	{
		return;
	}

	auto* Owner = GetOwningPlayer();
	if (!IsValid(Owner))
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	// 커브데이터 시작시간 끝시간 얻기
	float Start, End;
	mDecreaseCurve->GetTimeRange(Start, End);

	// 델타 타임 누적
	mAccDelta += World->GetDeltaSeconds();
	// 델타타임 누적으로 알파값 얻기

	mAccDelta = FMath::Clamp<float>(mAccDelta, Start, End);
	float Alpha = mDecreaseCurve->GetFloatValue(mAccDelta);

	// 누적시간을 바탕으로 현재 값 Currnet 값 만드릭
	mCurrentHp = FMath::Lerp<float>(mCurrentHp, mTargetValue, Alpha);
	BackProgressBar->SetPercent(mCurrentHp / mMaxHp);

	if (mAccDelta >= End)
	{
		mAccDelta = 0;
		ClearTimer();
	}
	else
	{
		// Decrease 함수가 가상 함수가 아니거나 일반 멤버 함수일 때
		World->GetTimerManager().SetTimer(mProgressBarUpdateTimerHandle, this, &UDoubleProgressBar::Decrease, 0.01f, false);
	}
}

void UDoubleProgressBar::ClearTimer()
{
	UWorld* World = GetWorld();
	if (false == IsValid(World))
	{
		return;
	}

	World->GetTimerManager().ClearTimer(mProgressBarUpdateDelay);
	World->GetTimerManager().ClearTimer(mProgressBarUpdateTimerHandle);
}

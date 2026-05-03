// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/StatusInfoWidget.h"

void UStatusInfoWidget::Init(const FString& AttributeName, const FStatUISnapshot& Snapshot)
{
	CTB_AttributeName->SetText(FText::FromString(AttributeName));
	CTB_CurrentValue->SetText(FText::AsNumber(Snapshot.CurrentValue));
}

void UStatusInfoWidget::Update(const FStatUISnapshot& Snapshot)
{
	if (Snapshot.bUsePreview)
	{
		CTB_CurrentValue->SetText(FText::AsNumber(Snapshot.PreviewValue));

		CTB_DiffAmount->SetText(FText::AsNumber(Snapshot.DiffValue));
		CTB_DiffAmount->SetVisibility(ESlateVisibility::Visible);
		SetTextColor(Snapshot.DiffValue >= 0);

		if (Snapshot.DiffValue != 0)
		{
			FLinearColor Color = Snapshot.DiffValue >= 0 ? mPlusColor : mMinusColor;
			Img_Diff->SetColorAndOpacity(Color);

			float Angle = Snapshot.DiffValue >= 0 ? 0.f : 180.f;
			Img_Diff->SetRenderTransformAngle(Angle);

			CTB_DiffAmount->SetText(FText::AsNumber(Snapshot.DiffValue));
			CTB_DiffAmount->SetColorAndOpacity(Color);
			Img_Diff->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Img_Diff->SetVisibility(ESlateVisibility::Collapsed);
		}

		
	}
	else
	{
		CTB_CurrentValue->SetText(FText::AsNumber(Snapshot.CurrentValue));
		CTB_DiffAmount->SetVisibility(ESlateVisibility::Collapsed);
		Img_Diff->SetVisibility(ESlateVisibility::Collapsed);
		SetTextColor(true);
	}
}

void UStatusInfoWidget::CallbackAttributeChanged(const FOnAttributeChangeData& ChangedData)
{
	// 인벤토리 상화작용과 별개로, 플레이의 어트리뷰트가 변경됐을때 즉각적으로 변동되는 값
	// 현재 어트리뷰트의 대부분의 값이 Current / Max 가 있는데, Current 가 바뀌는 것을 위함
	//UE_LOG(LogTemp, Error, TEXT("Attribute %s"), *ChangedData.Attribute.GetName());
	CTB_CurrentValue->SetText(FText::AsNumber(ChangedData.NewValue));
	SetTextColor(true);
}

void UStatusInfoWidget::SetTextColor(bool bIsPlus)
{
	FLinearColor Color = bIsPlus ? mPlusColor : mMinusColor;

	CTB_CurrentValue->SetColorAndOpacity(Color);
	CTB_DiffAmount->SetColorAndOpacity(Color);
}

void UStatusInfoWidget::UpdateValue(float Value)
{
	CTB_CurrentValue->SetText(FText::AsNumber(Value));
	SetTextColor(true);
}















// Legacy

void UStatusInfoWidget::UpdateCurrentValue(float Value)
{
	mCachCurrentValue = Value;
	CTB_CurrentValue->SetText(FText::AsNumber(mCachCurrentValue));
}

void UStatusInfoWidget::SetPreViewValue(float Value)
{
	float Diff = Value - mCachCurrentValue;
	FLinearColor Color = Diff >= 0 ? mPlusColor : mMinusColor;

	CTB_CurrentValue->SetText(FText::AsNumber(Value));
	CTB_CurrentValue->SetColorAndOpacity(Color);
}

void UStatusInfoWidget::SetDiffValue(float Value)
{
	if (Value == 0)
	{
		Img_Diff->SetVisibility(ESlateVisibility::Collapsed);
		CTB_DiffAmount->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	else
	{
		Img_Diff->SetVisibility(ESlateVisibility::HitTestInvisible);
		CTB_DiffAmount->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	FLinearColor Color = Value >= 0 ? mPlusColor : mMinusColor;
	Img_Diff->SetColorAndOpacity(Color);

	float Angle = Value > 0 ? 0.f : 180.f;
	Img_Diff->SetRenderTransformAngle(Angle);

	CTB_DiffAmount->SetText(FText::AsNumber(Value));
	CTB_DiffAmount->SetColorAndOpacity(Color);
}

void UStatusInfoWidget::SetDefault()
{
	UpdateCurrentValue(mCachCurrentValue);

	Img_Diff->SetVisibility(ESlateVisibility::Collapsed);
	CTB_DiffAmount->SetVisibility(ESlateVisibility::Collapsed);
	CTB_CurrentValue->SetColorAndOpacity(mPlusColor);

}

void UStatusInfoWidget::SetCurrentValue()
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RadinalPannel/RadinalPannelWidget.h"
#include "Kismet/KismetMathLibrary.h"

void URadinalPannelWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void URadinalPannelWidget::NativeConstruct()
{
	Super::NativeConstruct();


	UMaterialInstance* MatInst = Cast<UMaterialInstance>(Img_RadinalWheel->GetBrush().GetResourceObject());
	if (IsValid(MatInst))
	{
		mRadinalWheelDynamicMaterialInstance = UMaterialInstanceDynamic::Create(MatInst, this);
		check(mRadinalWheelDynamicMaterialInstance);
		Img_RadinalWheel->SetBrushFromMaterial(mRadinalWheelDynamicMaterialInstance);

		mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("SectionCount"), mSectionCount);
	}
	mSectionSize = 360.f / mSectionCount;

	for (const auto& Btn : mBtnArr)
	{
		Btn->mOnHoveredButton.AddDynamic(this, &URadinalPannelWidget::OnHoveredCallback_Btn);
		Btn->mOnUnHoveredButton.AddDynamic(this, &URadinalPannelWidget::OnUnHoveredCallback_Btn);
	}

	float HalfSize = mSectionSize / 2.f;

	for (int i = 0; i < mSectionCount; ++i)
	{
		mMaxRange.Add(i * mSectionSize + HalfSize);
	}

	for (const float& Value : mMaxRange)
	{
		float Temp = Value - mSectionSize;
		if (Temp < 0)
		{
			Temp += 360;
		}
		mMinRange.Add(Temp);
	}

	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		PC->bShowMouseCursor = true;
	}
}

float URadinalPannelWidget::GetMouseRotation()
{
	APlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		double MouseX, MouseY;
		PC->GetMousePosition(MouseX, MouseY);

		PC->bShowMouseCursor = true;

		FVector MouseLocation(MouseX, MouseY, 0);

		int32 ViewPortX, ViewPortY;
		PC->GetViewportSize(ViewPortX, ViewPortY);
		FVector ViewPortSize(ViewPortX / 2.f, ViewPortY / 2.f, 0);

		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(MouseLocation, ViewPortSize);
		return 180 - Rot.Yaw;
	}

	return -1.f;
}

void URadinalPannelWidget::ChangeMaterialParameterValue(float Value, float Opacity)
{
	check(mRadinalWheelDynamicMaterialInstance);

	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::FromInt(Value));
	mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("InnerActiveRotation"), Value);
	mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("SelectedRotation"), Value);
	mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("OuterActiveRotation"), Value);

	mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("InnerOpacity"), Opacity);
	mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("OuterOpacity"), Opacity);
	mRadinalWheelDynamicMaterialInstance->SetScalarParameterValue(TEXT("SelectedOpacity"), Opacity);

	if (0.0f == Opacity)
	{
		mRadinalWheelDynamicMaterialInstance->SetVectorParameterValue(TEXT("SelectedColor"), FVector4(Opacity));
	}
	else
	{
		mRadinalWheelDynamicMaterialInstance->SetVectorParameterValue(TEXT("SelectedColor"), FVector4(0.2f, 0.2f, 0.2f, 1.f));
	}
}

void URadinalPannelWidget::OnHoveredCallback_Btn()
{
	float Rotation = GetMouseRotation();

	int Count = mMinRange.Num();
	for (int i = 0; i < Count; ++i)
	{
		float Min = mMinRange[i];
		float Max = mMaxRange[i];

		if (Min > Max)
		{
			if (false == (Max < Rotation && Rotation < Min))
			{
				mCurrentSelectedSlotIdx = i;
				break;
			}
		}
		else
		{
			if (Min < Rotation && Rotation < Max)
			{
				mCurrentSelectedSlotIdx = i;
				break;
			}
		}
	}

	const FString& DebugMsg = FString::Printf(TEXT("Rot %f, Idx %d"), Rotation, mCurrentSelectedSlotIdx);
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, DebugMsg);

	float Value = mCurrentSelectedSlotIdx * mSectionSize /*- (mSectionSize / 2.f)*/;

	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::FromInt(Value));
	ChangeMaterialParameterValue(Value, 1.f);
}

void URadinalPannelWidget::OnUnHoveredCallback_Btn()
{
	ChangeMaterialParameterValue(0.f, 0.f);
}


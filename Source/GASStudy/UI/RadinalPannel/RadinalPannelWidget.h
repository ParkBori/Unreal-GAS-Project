// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "UI/RadinalPannel/RadinalButtonWidget.h"
#include "RadinalPannelWidget.generated.h"

/**
 * 
 */

UCLASS()
class GASSTUDY_API URadinalPannelWidget : public UUserWidget
{
	GENERATED_BODY()
	
#define INNER_ROTATION TEXT("InnerActiveRotation")
#define SELECTED_ROTATION TEXT("SelectedRotation")
#define OUTER_ROTATION TEXT("OuterActiveRotation")

protected:
	//virtual void NativeOnInitialized();
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	float GetMouseRotation();
	void ChangeMaterialParameterValue(float Value, float Opacity);

private:
	UFUNCTION()
	void OnHoveredCallback_Btn();
	
	UFUNCTION()
	void OnUnHoveredCallback_Btn();

public:
	TArray<TObjectPtr<URadinalButtonWidget>>& GetButtons() 
	{
		return mBtnArr;
	}

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_RadinalWheel;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<UCanvasPanel> Canvas_Btns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URadinalButtonWidget> mBtnClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<URadinalButtonWidget>> mBtnArr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 mSectionCount = 8;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> mRadinalWheelDynamicMaterialInstance;

	UPROPERTY(EditDefaultsOnly)
	float mPositionOffset = 250.f;

	float mSectionSize;

	int32 mCurrentSelectedSlotIdx;

	FVector mStartAxis = FVector(0.f, 1.f, 0.f);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D mBtnSize = FVector2D(300	, 300);

	TArray<float> mMinRange;
	TArray<float> mMaxRange;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "Shared/Inventory/Snapshot.h"
#include "GameplayEffectTypes.h"

#include "StatusInfoWidget.generated.h"

/**
 * 기본적으로 어트리뷰트는 Non-Temp Value 를 반영하고
 * 인벤토리에 의한 임시적으로 보여지는 값은 Temp Value 를 반영한다
 */
UCLASS()
class GASSTUDY_API UStatusInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(const FString& AttributeName, const FStatUISnapshot& Snapshot);
	void Update(const FStatUISnapshot& Snapshot);

	void CallbackAttributeChanged(const FOnAttributeChangeData& ChangedData);
	
	void SetTextColor(bool bIsPlus);

	void UpdateValue(float Value);

	// Legacy
	void UpdateCurrentValue(float Value);
	void SetPreViewValue(float Value);
	void SetDiffValue(float Value);
	void SetDefault();
	void SetCurrentValue();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> CTB_AttributeName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_DiffAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_CurrentValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Diff;

protected:
	UPROPERTY(EditAnywhere)
	FLinearColor mPlusColor;

	UPROPERTY(EditAnywhere)
	FLinearColor mMinusColor;

private:
	float mCachCurrentValue;
};

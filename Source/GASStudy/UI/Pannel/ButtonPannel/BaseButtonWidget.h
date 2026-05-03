// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIInfo.h"
#include "CommonButtonBase.h"
#include "BaseButtonWidget.generated.h"

/**
 * 외형적으로 최대한 많은 기능을 갖게 하여 다양하게 상용
 */
UCLASS()
class GASSTUDY_API UBaseButtonWidget : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	void OnHoveredCallback();
	void OnUnHoveredCallback();

	void ChangeStyle(bool bHovered);


public:
	void SetBtnName(FText BtnName);

protected:
	// 버튼 내부에 존재하는 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_BtnName;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Style")
	FText mButtonNameText;

public:
	UPROPERTY(EditAnywhere, Category = "Style")
	FButtonTextStyle mTextStyle;
};

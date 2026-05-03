// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/UIInfo.h"
#include "ContentPannelWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UContentPannelWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeDestruct() override;

public:
	EVerticalAlignment GetVerticalAlignment() const
	{
		return mVerticalAlignment;
	}

	EHorizontalAlignment GeTHorizontalAlignment() const
	{
		return mHorizontalAlignment;
	}

public:
	void SetBasePanelWidget(UUserWidget* BasePanelWidget)
	{
		mBasePanelWidget = BasePanelWidget;
	}

protected:


protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> mBasePanelWidget;

protected:
	UPROPERTY(EditAnywhere, Category = "Alignment")
	TEnumAsByte<EVerticalAlignment> mVerticalAlignment = EVerticalAlignment::VAlign_Fill;

	UPROPERTY(EditAnywhere, Category = "Alignment")
	TEnumAsByte<EHorizontalAlignment> mHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;


};

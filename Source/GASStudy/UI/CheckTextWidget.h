// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "CheckTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UCheckTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	void SetCheckBox(bool bCheck);
	void SetText(const FText& Desc);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> CB_IsComplete;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Description;
};

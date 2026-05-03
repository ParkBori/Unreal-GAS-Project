// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "UI/BaseUserWidget.h"
#include "GameClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGameClearWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UGameClearWidget(const FObjectInitializer& ObjectInitializer);

	void ShowGameCleareWidget();

	UFUNCTION()
	void HiddenGameClearWidget();

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation>  ActivateImg;
};

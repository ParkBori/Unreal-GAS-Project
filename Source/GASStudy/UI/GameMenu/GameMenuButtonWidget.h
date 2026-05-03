// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameMenuPublicInfo.h"
#include "GameMenuButtonWidget.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FButtonSelectd, EGameMenuType, Type);

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGameMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameMenuButtonWidget(const FObjectInitializer& ObjectInitializer);

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;


	void PlayWidgetAnimation(const FString& Name, float PlayStartTime = 0.f,
		float PlaySpeed = 1.f, bool Forward = true, bool RestoreState = false,
		int32 LoopCount = 1);

	UFUNCTION()
	void HoverButton();

	UFUNCTION()
	void UnHoverButton();

public:
	UPROPERTY(EditAnywhere)
	EGameMenuType ButtonType;

	FButtonSelectd OnButtonSelect;

protected:
	UPROPERTY()
	TObjectPtr<UButton> Button;

	UPROPERTY()
	TMap<FString, TObjectPtr<UWidgetAnimation>>	ButtonAnimation;
};

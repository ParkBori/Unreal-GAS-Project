// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "UI/BaseUserWidget.h"
#include "StartSceneWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UStartSceneWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UStartSceneWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnHovered_StartButton();

	UFUNCTION()
	void OnUnHovered_StartButton();

	UFUNCTION()
	void OnClicked_StartButton();


protected:
	UPROPERTY()
	TObjectPtr<class UMediaPlayer>	mMediaPlayer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UFileMediaSource> mMediaSource;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UButton> mStartButton;
};

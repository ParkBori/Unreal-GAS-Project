// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Pannel/BasePannel/BasePannelWidget.h"
#include "SettingBasePannelWidget.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnDestructWidget);

class UBaseButtonWidget;
/**
 * 
 */
UCLASS()
class GASSTUDY_API USettingBasePannelWidget : public UBasePannelWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnClickedCallback_CBtn_Reset();
	virtual void OnClickedCallback_CBtn_Apply();
	virtual void OnClickedCallback_CBtn_Back();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Reset;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Apply;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Back;

private:
	UPROPERTY()
	TScriptInterface<class IContentPanenlUIInterface> mContentInterface;

public:
	FOnDestructWidget mOnDestructWidget;
};

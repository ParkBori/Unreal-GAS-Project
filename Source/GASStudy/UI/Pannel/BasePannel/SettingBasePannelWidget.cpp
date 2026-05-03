// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/BasePannel/SettingBasePannelWidget.h"
#include "Interface/ContentPanenlUIInterface.h"
#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"

void USettingBasePannelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Reset))
	{
		Btn_Reset->OnClicked().AddUObject(this, &USettingBasePannelWidget::OnClickedCallback_CBtn_Reset);
	}

	if (IsValid(Btn_Apply))
	{
		Btn_Apply->OnClicked().AddUObject(this, &USettingBasePannelWidget::OnClickedCallback_CBtn_Apply);
	}

	if (IsValid(Btn_Back))
	{
		Btn_Back->OnClicked().AddUObject(this, &USettingBasePannelWidget::OnClickedCallback_CBtn_Back);;
	}
}

void USettingBasePannelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (IsValid(mContentWidget))
	{
		if (mContentWidget->Implements<UContentPanenlUIInterface>())
		{
			mContentInterface = TScriptInterface<IContentPanenlUIInterface>(mContentWidget);
		}
	}
}

void USettingBasePannelWidget::NativeDestruct()
{
	if (mOnDestructWidget.IsBound())
	{
		mOnDestructWidget.Broadcast();
	}
}

void USettingBasePannelWidget::OnClickedCallback_CBtn_Reset()
{
	if (mContentInterface)
	{
		mContentInterface->Reset();
	}
}

void USettingBasePannelWidget::OnClickedCallback_CBtn_Apply()
{
	if (mContentInterface)
	{
		mContentInterface->ApplyChange();
	}
}

void USettingBasePannelWidget::OnClickedCallback_CBtn_Back()
{
	if (mContentInterface)
	{
		mContentInterface->Back();
	}
}

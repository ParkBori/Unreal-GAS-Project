// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ContentPannel/SettingContentPannelWidget.h"
#include "UI/Pannel/ButtonPannel/OptionCycleButtonWidget.h"

void USettingContentPannelWidget::Back()
{

}

void USettingContentPannelWidget::Broadcast_Settings()
{
	bool bSettiingChanged = CheckIsAnySettingDirty();
	if (bSettiingChanged)
	{
		if (bHasChanges)
		{
			BroadCast_OnAnySettingChanged();
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("오염됨"));
		}

		else
		{
			BroadCast_OnAnySettingUnChanged();
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("원복"));
		}
	}
}

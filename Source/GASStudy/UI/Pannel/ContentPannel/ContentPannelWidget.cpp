// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ContentPannel/ContentPannelWidget.h"

void UContentPannelWidget::NativeDestruct()
{
	if (IsValid(mBasePanelWidget))
	{
		mBasePanelWidget->RemoveFromParent();
	}

	Super::NativeDestruct();
}


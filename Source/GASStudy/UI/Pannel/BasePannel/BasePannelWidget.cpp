// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/BasePannel/BasePannelWidget.h"
#include "UI/Pannel/ContentPannel/ContentPannelWidget.h"

void UBasePannelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBasePannelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(mContentClass))
	{
		
		UContentPannelWidget* Content = CreateWidget<UContentPannelWidget>(this, mContentClass);
		if (IsValid(Content))
		{
			if (IsValid(OV_Content))
			{
				auto OVSlot = OV_Content->AddChildToOverlay(Content);
				if (IsValid(OVSlot))
				{
					OVSlot->SetHorizontalAlignment(Content->GeTHorizontalAlignment());
					OVSlot->SetVerticalAlignment(Content->GetVerticalAlignment());
				}
			}
			mContentWidget = Content;
			Content->SetBasePanelWidget(this);
		}
	}
}

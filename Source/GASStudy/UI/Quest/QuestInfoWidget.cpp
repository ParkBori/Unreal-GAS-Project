// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestInfoWidget.h"
#include "Component/QuestComponent.h"

#include "UI/Quest/QuestRewardWidget.h"
#include "UI/CheckTextWidget.h"
#include "UI/LineTextWidget.h"

void UQuestInfoWidget::Init(UQuestComponent* QuestComponent)
{
	mQuestComponent = QuestComponent;

	mQuestComponent->mOnUpdatedTrackingQuest.RemoveAll(this);
	mQuestComponent->mOnUpdatedTrackingQuest.AddUObject(this, &UQuestInfoWidget::UpdateQuestData);
}

void UQuestInfoWidget::UpdateQuestData(const FQuestSnapshot& Snapshot)
{
	if (Snapshot.QuestName.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}

	TB_QuestName->SetText(Snapshot.QuestName);
	TB_QuestDesc->SetText(Snapshot.QuestDescript);
	TB_QuestState->SetText(Snapshot.QuestState);
	if (Snapshot.bIsCompletedQuest)
	{
		SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.f));
	}
	else
	{
		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	const auto& Reward = Snapshot.Reward;
	for (auto& Child : HB_Reward->GetAllChildren())
	{
		HB_Reward->RemoveChild(Child);
	}

	for (const auto& Item : Reward.Items)
	{
		UQuestRewardWidget* RW = CreateWidget<UQuestRewardWidget>(GetOwningPlayer(), mRewardBox);
		if (RW)
		{
			RW->SetName(FText::FromName(Item.Key));
			RW->SetImage(Item.Value.ItemIcon);
		}
		
		auto* HS = HB_Reward->AddChildToHorizontalBox(RW);
		if (HS)
		{
			HS->SetPadding(FMargin(0.f, 0.f, 25.f, 0.f));
		}
	}


	// 스크롤 박스 비워주기
	SB_QuestProgress->ClearChildren();

	Snapshot.QuestProgress;
	for (auto& Progress : Snapshot.QuestProgress)
	{
		bool bDrawLine = !Progress.bCurrent;

		ULineTextWidget* LT = CreateWidget<ULineTextWidget>(GetOwningPlayer(), mLineText);
		if (!LT)
		{
			return;
		}

		LT->SetText(Progress.StageName);
		
		if (!Snapshot.bIsCompletedQuest)
		{
			LT->DrawLine(bDrawLine);
		}
		else
		{
			LT->DrawLine(false);
		}
		
		auto* SLHead = Cast<UScrollBoxSlot>(SB_QuestProgress->AddChild(LT));
		if (SLHead)
		{
			SLHead->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		}

		for (const auto& DetailData : Progress.QuestUIDetailDataList)
		{
			if (DetailData.Description.IsEmpty())
			{
				continue;
			}
			
			ULineTextWidget* LT2 = CreateWidget<ULineTextWidget>(GetOwningPlayer(), mLineText);
			LT2->SetText(DetailData.Description);
			LT2->DrawLine(DetailData.bCompleted);
			
			auto* SL = Cast<UScrollBoxSlot>(SB_QuestProgress->AddChild(LT2));
			if (SL)
			{
				SL->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
				SL->SetPadding(FMargin(30.f, 0.f, 0.f, 0.f));
			}
			// DetailData.DetailProgress;
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestListWidget.h"
#include "Component/QuestComponent.h"

#include "UI/LineTextWidget.h"
#include "UI/Quest/QuestEntityWidget.h"


void UQuestListWidget::Init(UQuestComponent* QuestComponent)
{
	mQuestComponent = QuestComponent;
	
	mQuestComponent->mOnUpdatedQuestList.RemoveAll(this);
	mQuestComponent->mOnUpdatedShowingQuest.RemoveAll(this);

	mQuestComponent->mOnUpdatedQuestList.AddUObject(this, &UQuestListWidget::UpdateQuestList);
	mQuestComponent->mOnUpdatedShowingQuest.AddUObject(this, &UQuestListWidget::UpdatedShowingQuest);
}

bool UQuestListWidget::ResetVisual()
{
	for (auto& Data: mActiveQuestWidget)
	{
		if (Data.Key == mTrackingQuestID)
		{
			Data.Value->bIsHovered = false;
			Data.Value->bIsSelected = true;
			Data.Value->bIsTracking = true;

			mCurrentActiveQuestID = mTrackingQuestID;
			Data.Value->RequestQuestData();
		}
		else
		{
			Data.Value->bIsHovered = false;
			Data.Value->bIsSelected = false;
		}
		
		Data.Value->RefreshVisual();
	}

	for (auto& Data : mCompleteQuestWidget)
	{
		Data.Value->bIsHovered = false;
		Data.Value->bIsSelected = false;

		Data.Value->RefreshVisual();
	}

	return mActiveQuestWidget.IsEmpty();
}

void UQuestListWidget::UpdateQuestList(const FQuestListSnapshot& Snapshot)
{
	bool bIsTracking = false;

	SB_ActivateQuest->ClearChildren();
	mActiveQuestWidget.Empty();
	for (const FName& ActiveQuestName : Snapshot.ActiveQuestList)
	{
		UQuestEntityWidget* QE = CreateWidget<UQuestEntityWidget>(GetOwningPlayer(), mQuestEntityWidget);
		if (QE)
		{
			bIsTracking = (ActiveQuestName == Snapshot.CurrentTrackingQuest);
			if (bIsTracking)
			{
				mTrackingQuestID = mCurrentActiveQuestID = Snapshot.CurrentTrackingQuest;
			}

			QE->Init(ActiveQuestName, false,  bIsTracking);
			UScrollBoxSlot* SB = Cast<UScrollBoxSlot>(SB_ActivateQuest->AddChild(QE));
			if (SB)
			{
				SB->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
			}

			mActiveQuestWidget.Add(ActiveQuestName, QE);
		}
	}

	SB_CompleteQuest->ClearChildren();
	mCompleteQuestWidget.Empty();
	for (const FName& CompleteQuestName : Snapshot.CompleteQuestList)
	{
		UQuestEntityWidget* QE = CreateWidget<UQuestEntityWidget>(GetOwningPlayer(), mQuestEntityWidget);
		if (QE)
		{
			QE->Init(CompleteQuestName, true, false);
			UScrollBoxSlot* SB = Cast<UScrollBoxSlot>(SB_CompleteQuest->AddChild(QE));
			if (SB)
			{
				SB->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
			}

			mCompleteQuestWidget.Add(CompleteQuestName, QE);
		}
	}
}

void UQuestListWidget::UpdatedShowingQuest(const FName& QuestID, bool bChangedTrackingQuest)
{
	// 이전 Selected 해제
	if (mCurrentActiveQuestID != NAME_None &&
		mCurrentActiveQuestID != QuestID)
	{
		if (UQuestEntityWidget* Prev = GetQuestWidget(mCurrentActiveQuestID))
		{
			Prev->bIsSelected = false;
			Prev->RefreshVisual();
		}
	}

	// Tracking 변경
	if (bChangedTrackingQuest &&
		mTrackingQuestID != NAME_None &&
		mTrackingQuestID != QuestID)
	{
		if (UQuestEntityWidget* PrevTracking = GetQuestWidget(mTrackingQuestID))
		{
			PrevTracking->bIsTracking = false;
			PrevTracking->RefreshVisual();
		}

		mTrackingQuestID = QuestID;
	}

	// 현재 Selected 설정
	if (UQuestEntityWidget* Current = GetQuestWidget(QuestID))
	{
		Current->bIsSelected = true;

		if (bChangedTrackingQuest)
		{
			Current->bIsTracking = true;
		}

		Current->RefreshVisual();
	}

	mCurrentActiveQuestID = QuestID;
}

UQuestEntityWidget* UQuestListWidget::GetQuestWidget(const FName& ID)
{
	if (mActiveQuestWidget.Contains(ID))
		return mActiveQuestWidget[ID].Get();

	if (mCompleteQuestWidget.Contains(ID))
		return mCompleteQuestWidget[ID].Get();

	return nullptr;
}
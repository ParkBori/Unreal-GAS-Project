// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/HUDQuestTrackerWidget.h"
#include "Component/QuestComponent.h"
#include "UI/LineTextWidget.h"

void UHUDQuestTrackerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (auto* QC = OwningPawn->FindComponentByClass<UQuestComponent>())
		{
			// 중복 바인딩 방지를 위해 먼저 Unbind 하거나 체크
			QC->mOnUpdatedTrackingQuest.RemoveAll(this);
			QC->mOnUpdatedTrackingQuest.AddUObject(this, &ThisClass::UpdateQuestData);
		}
	}
}

void UHUDQuestTrackerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDQuestTrackerWidget::UpdateQuestData(const FQuestSnapshot& Snapshot)
{
	if (Snapshot.QuestName.IsEmpty() || !mLineText || !Snapshot.bIsTrackingQuest)
	{
		return;
	}


	SB_QuestProgress->ClearChildren();

	Snapshot.QuestProgress;
	for (auto& Progress : Snapshot.QuestProgress)
	{
		if (!Progress.bCurrent)
		{
			continue;
		}

		ULineTextWidget* LT = CreateWidget<ULineTextWidget>(GetOwningPlayer(), mLineText);
		if (!LT)
		{
			return;
		}

		LT->SetText(Progress.StageName);
		LT->DrawLine(false);

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

			FString Msg = DetailData.bCompleted ? TEXT("True") : TEXT("False");
			UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);

			LT2->DrawLine(DetailData.bCompleted);

			auto* SL = Cast<UScrollBoxSlot>(SB_QuestProgress->AddChild(LT2));
			if (SL)
			{
				SL->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
				SL->SetPadding(FMargin(30.f, 0.f, 0.f, 0.f));
			}
		}
	}
}

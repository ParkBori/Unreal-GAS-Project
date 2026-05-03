// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestEntityWidget.h"
#include "Subsystem/QuestSubsystem.h"


void UQuestEntityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_MouseEvent->OnHovered.AddDynamic(this, &UQuestEntityWidget::Callback_Hover);
	Btn_MouseEvent->OnUnhovered.AddDynamic(this, &UQuestEntityWidget::Callback_UnHover);
	Btn_MouseEvent->OnClicked.AddDynamic(this, &UQuestEntityWidget::Callback_Click);
}

void UQuestEntityWidget::Init(const FName& QuestID, bool bIsCompleted, bool bTracking)
{
	mQuestID = QuestID;
	TB_QuestName->SetText(FText::FromName(mQuestID));

	mIsCompleted = bIsCompleted;
	if (mIsCompleted)
	{
		SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.f));
	}

	if (bTracking)
	{
		bIsTracking = bTracking;
		bIsSelected = bTracking;
		// TODO : 색상 변경 기능 필요
		RefreshVisual();
	}
}

void UQuestEntityWidget::Activate(bool bCurrentTracking)
{
	SetRenderScale(FVector2D(1.3f, 1.3f));
	Img_Active->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UQuestEntityWidget::DeActivate(bool bChangedTrackingQuest)
{
	SetRenderScale(FVector2D(1.f, 1.f));
	
	if (bChangedTrackingQuest)
	{
		Img_Active->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UQuestEntityWidget::RefreshVisual()
{
	float Scale = 1.0f;

	if (bIsHovered || bIsSelected)
	{
		Scale = 1.3f;
	}

	SetRenderScale(FVector2D(Scale));

	Img_Active->SetVisibility(
		bIsTracking ? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Collapsed
	);
}

void UQuestEntityWidget::RequestQuestData()
{
	UQuestSubsystem* QS = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (QS)
	{
		QS->RequestQuestData(mQuestID, false);
	}
}

void UQuestEntityWidget::Callback_Hover()
{
	bIsHovered = true;
	RefreshVisual();

	UQuestSubsystem* QS = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (QS)
	{
		//QS->RequestChangeTrackingQuest(mQuestID);
		QS->RequestQuestData(mQuestID, false);
	}
}

void UQuestEntityWidget::Callback_UnHover()
{
	bIsHovered = false;
	RefreshVisual();
}

void UQuestEntityWidget::Callback_Click()
{
	UQuestSubsystem* QS = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (QS)
	{
		//QS->RequestChangeTrackingQuest(mQuestID);

		// 이미 클리어된 퀘스트는, 추적중인 퀘스트로 설정이 불가능하고
		// 오직 볼수만 있음
		bool bChangeTrackingQuest = mIsCompleted ? false : true;
		if (bChangeTrackingQuest)
		{
			//Img_Active->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		QS->RequestQuestData(mQuestID, bChangeTrackingQuest);
	}
}



//호버시 보여주는 퀘스트 정보가 달라지고 
//
//클릭스 추적중인 퀘스트 변경 -> 퀘스트 정보변경
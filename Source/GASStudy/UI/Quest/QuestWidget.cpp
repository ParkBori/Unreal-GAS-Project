// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestWidget.h"

#include "Component/QuestComponent.h"

#include "UI/Quest/QuestListWidget.h"
#include "UI/Quest/QuestInfoWidget.h"

void UQuestWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UQuestWidget::Init(class UQuestComponent* QuestComponent)
{
	check(QuestComponent);
	mQuestComponent = QuestComponent;

	if (IsValid(QuestList))
	{
		QuestList->Init(mQuestComponent.Get());
	}

	if (IsValid(QuestInfo))
	{
		QuestInfo->Init(mQuestComponent.Get());
	}
}

void UQuestWidget::Refresh()
{
	if (QuestList->ResetVisual())
	{
		QuestInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
}
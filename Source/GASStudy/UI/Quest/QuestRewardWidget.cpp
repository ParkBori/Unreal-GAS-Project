// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestRewardWidget.h"

void UQuestRewardWidget::SetName(const FText& Name)
{
	TB_ItemName->SetText(Name);
}

void UQuestRewardWidget::SetImage(UTexture2D* Icon)
{
	Img_Icon->SetBrushFromTexture(Icon);
}

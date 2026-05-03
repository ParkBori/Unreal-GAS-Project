// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/LoadPannel/LoadEntityWidget.h"

void ULoadEntityWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Btn)
	{
		Btn->OnClicked.AddDynamic(this, &ThisClass::Callback_Clicked);
	}
}

void ULoadEntityWidget::Init(int32 Idx, const FString& SaveSlotName, const FString& Date)
{
	mSaveSlotName = *SaveSlotName;

	TB_SlotName->SetText(FText::FromString(FString::Printf(TEXT("backup_%02d.sav"), Idx)));
	TB_Date->SetText(FText::FromString(Date));
}

void ULoadEntityWidget::VisualUpdate()
{
	if (bSelected)
	{
		B_Cover->SetBrushColor(FLinearColor(1.f, 0.f, 0.f, 0.2f));
	}
	else
	{
		B_Cover->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.2f));
	}
}

void ULoadEntityWidget::Callback_Clicked()
{
	mOnEntitySelected.Broadcast(mSaveSlotName);
}





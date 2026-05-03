// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LineTextWidget.h"

void ULineTextWidget::DrawLine(bool bActivate)
{
	bActivate ? Img_Line->SetVisibility(ESlateVisibility::SelfHitTestInvisible) : Img_Line->SetVisibility(ESlateVisibility::Collapsed);
}

void ULineTextWidget::SetText(const FText& Text)
{
	TB_Content->SetText(Text);
}

void ULineTextWidget::SetFontSize(int32 Size)
{
	FSlateFontInfo NewFontInfo = TB_Content->GetFont();

	NewFontInfo.Size = Size;

	TB_Content->SetFont(NewFontInfo);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "LineTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ULineTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DrawLine(bool bActivate);
	void SetText(const FText& Text);
	void SetFontSize(int32 Size);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Content;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Line;
};

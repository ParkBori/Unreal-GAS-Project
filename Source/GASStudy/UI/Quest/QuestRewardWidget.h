// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "QuestRewardWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UQuestRewardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetName(const FText& Name);
	void SetImage(UTexture2D* Icon);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;
};

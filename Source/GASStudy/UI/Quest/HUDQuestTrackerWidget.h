// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "HUDQuestTrackerWidget.generated.h"

/**
 * 현재 퀘스트 스테이지 + 스테이지 목표 표시
 */
UCLASS()
class GASSTUDY_API UHUDQuestTrackerWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	

public:
	void UpdateQuestData(const struct FQuestSnapshot& Snapshot);



private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SB_QuestProgress;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULineTextWidget> mLineText;
	
};

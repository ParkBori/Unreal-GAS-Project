// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "GameplayTagContainer.h"
#include "QuestEntityWidget.generated.h"

/**
 * 
 */

class UQuestComponent;

UCLASS()
class GASSTUDY_API UQuestEntityWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

public:
	void Init(const FName& QuestID, bool bIsCompleted , bool bTracking = false);

	void Activate(bool bCurrentTracking);
	void DeActivate(bool bChangedTrackingQuest);

	void RefreshVisual();
	
	/**
	* 새롭게 창이 닫혔다가 열렸을때, 호출되는 함수 
	* 외부에서 호출
	*/
	void RequestQuestData();

private:
	UFUNCTION()
	void Callback_Hover();

	UFUNCTION()
	void Callback_UnHover();

	UFUNCTION()
	void Callback_Click();



public:
	bool bIsHovered = false;
	bool bIsSelected = false;      // 현재 보여지는 퀘스트
	bool bIsTracking = false;      // 추적중인 퀘스트

private:

	UPROPERTY()
	FName mQuestID;

	bool mIsCompleted;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage>  Img_Active;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_MouseEvent;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_QuestName;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Hover;
};

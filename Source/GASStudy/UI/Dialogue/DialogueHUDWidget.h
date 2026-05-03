// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "UI/Dialogue/DialogueChoiceWidget.h"

// ==== Legacy ====
#include "Dialogue/DialogueInfo.h"
// ==== Legacy ====

#include "Shared/Dialogue/DialogueData.h"

#include "DialogueHUDWidget.generated.h"


/**
 * 
 */
UCLASS()
class GASSTUDY_API UDialogueHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

protected:
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


public:
	/**
	* 다이얼로그 UI 업데이트 함수 
	*/
	void Update(const FUIDialogueData& Data);

private:
	/**
	* 기존 선택지 제거
	*/
	void RemoveChoice();

	/**
	* 다디얼로그 택스트 업데이트 
	*/
	void UpdateDialogueText(const FText& Speaker, const FText& Script);
	
	/**
	* 초이스 업데이트
	*/
	void CreateChoice_V2(const TArray<FUIChoiceData>& Choic);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CV_ChocieBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Dialogue;


private:
	/**
	* 선택지 존재 o -> 클릭으로 넘길 수 없음
	* 선택지 존재 X -> 클릭으로 넘길 수 있음
	*/
	bool bCanNextByLeftClick = false;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogueChoiceWidget> mDialogueChoiceWidget;

	UPROPERTY()
	TArray<TObjectPtr<UDialogueChoiceWidget>> mChoices;
};

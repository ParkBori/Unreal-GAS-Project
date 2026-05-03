// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Quest/QuestSnapshot.h"
#include "UI/UIInfo.h"
#include "QuestListWidget.generated.h"

/**
 * 
 */

class UQuestComponent;

UCLASS()
class GASSTUDY_API UQuestListWidget : public UUserWidget
{
	GENERATED_BODY()
	
	const int32 FONT_SIZE = 20;
	

public:
	void Init(UQuestComponent* QuestComponent);


public:
	/**
	* 현재 활성화 퀘스트 목록이 비여있음
	*/
	bool ResetVisual();

private:
	void UpdateQuestList(const FQuestListSnapshot& Snapshot);

	// 현재활성화된 퀘스트 목록의 퀘스트 엔티티 객체에 표시 
	// 활성화(마우스가 올라가있는) 퀘스트 확대 
	void UpdatedShowingQuest(const FName& QuestID, bool bChangedTrackingQuest);

	class UQuestEntityWidget* GetQuestWidget(const FName& ID);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SB_ActivateQuest;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SB_CompleteQuest;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULineTextWidget> mLineTextWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQuestEntityWidget> mQuestEntityWidget;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TWeakObjectPtr<UQuestComponent> mQuestComponent;


private:
	FName mTrackingQuestID;
	FName mCurrentActiveQuestID;

	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UQuestEntityWidget>> mActiveQuestWidget;

	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UQuestEntityWidget>> mCompleteQuestWidget;
};

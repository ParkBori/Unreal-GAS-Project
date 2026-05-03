// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Quest/QuestSnapshot.h"
#include "UI/UIInfo.h"
#include "QuestInfoWidget.generated.h"

/**
 * 
 */

class UQuestComponent;

UCLASS()
class GASSTUDY_API UQuestInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void Init(UQuestComponent* QuestComponent);

private:
	void UpdateQuestData(const FQuestSnapshot& Snapshot);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_QuestName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_QuestDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_QuestState;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Reward;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SB_QuestProgress;
	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQuestRewardWidget> mRewardBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULineTextWidget> mLineText;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCheckTextWidget> mObjectiveCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPreserveRatio))
	TWeakObjectPtr<UQuestComponent> mQuestComponent;
};

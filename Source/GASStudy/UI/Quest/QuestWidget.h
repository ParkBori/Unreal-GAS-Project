// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
class UQuestComponent;

UCLASS()
class GASSTUDY_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void Init(class UQuestComponent* QuestComponent);

	void Refresh();



protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UQuestListWidget> QuestList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UQuestInfoWidget> QuestInfo;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UQuestComponent> mQuestComponent;
};

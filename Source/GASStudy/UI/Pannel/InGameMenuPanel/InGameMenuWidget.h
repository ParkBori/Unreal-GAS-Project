// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenuWidget.generated.h"

/**
 * 
 */



UCLASS()
class GASSTUDY_API UInGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void Init();

private:
	UFUNCTION()
	void ChangedVisibility(ESlateVisibility InVisibility);

protected:
	void OnPreviewKeyDown();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> Inventory;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UQuestWidget> Quest;
};

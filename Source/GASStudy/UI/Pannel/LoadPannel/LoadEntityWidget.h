// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "LoadEntityWidget.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEntitySelected, const FName&);

/**
 * 
 */
UCLASS()
class GASSTUDY_API ULoadEntityWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void Init(int32 Idx, const FString& SaveSlotName, const FString& Date);
	
	const FName& GetSaveSlotName() const
	{
		return mSaveSlotName;
	}

	void SetSelected(bool bSelect)
	{
		bSelected = bSelect;
		VisualUpdate();
	}

private:
	void VisualUpdate();

	UFUNCTION()
	void Callback_Clicked();
	

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_SlotName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Date;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> B_Cover;

private:
	UPROPERTY()
	TWeakObjectPtr<class ULoadWidget> mParent;

	FName mSaveSlotName;

	bool bSelected = false;

public:
	FOnEntitySelected mOnEntitySelected;
};

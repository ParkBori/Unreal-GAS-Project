// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "LoadWidget.generated.h"


class ULoadEntityWidget;
class UBaseButtonWidget;


DECLARE_MULTICAST_DELEGATE(FOnClickedBackButn);

/**
 * 
 */
UCLASS()
class GASSTUDY_API ULoadWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;


public:
	void Reset();

private:
	void RefreshSaveList();

	void Callback_EntitySelected(const FName& SelectEntityName);

	UFUNCTION()
	void Callback_ClickedLoadBtn();
	UFUNCTION()
	void Callback_ClickedRemoveBtn();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SB_LoadEntityList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Load;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Remove;



private:
	UPROPERTY(Transient)
	TMap<FName, TWeakObjectPtr<ULoadEntityWidget>> mCachedLoadEntities;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadEntityWidget> mLoadEntityClass;

public:
	UPROPERTY(Transient)
	FName mSelectedSaveSlotName;

	FOnClickedBackButn mOnClickedBackButn;
};

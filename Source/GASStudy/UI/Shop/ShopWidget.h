// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Shared/Shop/ShopData.h"
#include "ShopWidget.generated.h"

class UShopItemListWidget;
class UBaseButtonWidget;




/**
 * 
 */
UCLASS()
class GASSTUDY_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override; 

public:
	UFUNCTION(BlueprintCallable)
	void Init(FName NPCName);

private:
	void Callback_ClickBuy();

	void Callback_ClickSell();

	void Callback_ClickExit();

	void Callback_NPCScirptCollapsTimerHandle();

	void Callback_ChangedMoney(float NewValue);

	/**
	* 판매 구매 결과에 따른 NPC 대화 출력 
	*/
	void Callback_ItemListInteract(EShopItemInteractResultType ResultType);


private:
	void ChangeShopMode(EShopModeType NewMode);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> CAS;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Buy;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Sell;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Exit;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UShopItemListWidget> BuyList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UShopItemListWidget> SellList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SB_Selector;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_NPCScript;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Gold;


protected:
	EShopModeType mCurrentMode = EShopModeType::None;
	FName mNPCName;

private:
	FTimerHandle mNPCScirptCollapsTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Shared/Shop/ShopData.h"
#include "ShopItemListWidget.generated.h"


class UBaseButtonWidget;






DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractResult, EShopItemInteractResultType);

// 구매 or 판매시 인벤토리에 변화가 생기면 판매리스트를 업데이트
// Sell 쪽에서만 실행 
DECLARE_MULTICAST_DELEGATE(FOnItemListChanged);

/**
 * 
 */
UCLASS()
class GASSTUDY_API UShopItemListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	void Init(FName NPCName, EShopItemListType Type);

	void RefreshItemList();

private:
	void CreateBuyList();

	void CreateSellList();

	void BuyItem();

	void SellItem();

	void Callback_ItemEntityClick(UObject* Item);

	void Callback_ClickInteract();
	
	void Callback_ClickShopMain();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ItemList;

	UPROPERTY(EditAnywhere, Category = "ShopItemListType")
	EShopItemListType mShopItemListType = EShopItemListType::None;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Interact;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_ShopMain;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> SB_Amount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Type;



	UPROPERTY()
	TWeakObjectPtr<class UShopItemItemData> mClickedData;

	FName mNPCName;

public:
	FOnInteractResult mOnInteractResult;

	FOnItemListChanged mOnItemListChanged;

	TFunction<void()> mGoShopMainFunc;
};

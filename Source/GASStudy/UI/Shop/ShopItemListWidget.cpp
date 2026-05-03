// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemListWidget.h"
#include "UI/Shop/ShopItemEntityWidget.h"
#include "Component/InventoryComponent_V2.h"
#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"
#include "DeveloperSettings/GameDataSettings.h"

void UShopItemListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(ItemList))
	{
		ItemList->SetSelectionMode(ESelectionMode::Single);

		// 아이템 클릭 이벤트 바인딩
		ItemList->OnItemClicked().AddUObject(this, &UShopItemListWidget::Callback_ItemEntityClick);
		//ItemList->OnItemSelectionChanged().AddUObject(this, &UShopItemListWidget::Callback_ItemEntityClick);
	}

	if (IsValid(Btn_Interact))
	{
		Btn_Interact->OnClicked().AddUObject(this, &UShopItemListWidget::Callback_ClickInteract);
	}

	if (IsValid(Btn_ShopMain))
	{
		Btn_ShopMain->OnClicked().AddUObject(this, &UShopItemListWidget::Callback_ClickShopMain);
	}

}

void UShopItemListWidget::Init(FName NPCName, EShopItemListType Type)
{
	mShopItemListType = Type;

	mNPCName = NPCName;

	FName BtnName;

	switch (Type)
	{
	case EShopItemListType::Buy:
		CreateBuyList();
		BtnName = TEXT("Buy");
		break;
	case EShopItemListType::Sell:
		CreateSellList();
		BtnName = TEXT("Sell");
		break;
	}
	Btn_Interact->SetBtnName(FText::FromName(BtnName));
	TB_Type->SetText(FText::FromString(BtnName.ToString() + TEXT(" List")));
}

void UShopItemListWidget::RefreshItemList()
{
	ItemList->ClearListItems();

	switch (mShopItemListType)
	{
	case EShopItemListType::Sell:
		CreateSellList();
		break;
	case EShopItemListType::Buy:
		CreateBuyList();
		break;
	}
}


void UShopItemListWidget::CreateBuyList()
{
	if (mNPCName.IsNone())
	{
		return;
	}

	auto GDS = UGameDataSettings::Get();
	if (!IsValid(GDS))
	{
		return;
	}

	const FNPCShopItemList* NPCShopItemList = GDS->GetDataTableRow<FNPCShopItemList>(EGlobalGameDataTableType::ShopNPCItemDat, mNPCName);
	if (!NPCShopItemList)
	{
		return;
	}

	for (const auto SID : NPCShopItemList->mShopItemData)
	{
		FName ItemID = SID.ItemID;

		const FItemInfoBase* ItemInfo = UGameDataSettings::Get()->GetDataTableRow<FItemInfoBase>(EGlobalGameDataTableType::ItemData, ItemID);
		if (!ItemInfo)
		{
			continue;
		}

		const UBaseItemDataAsset* DA = ItemInfo->ItemData;
		if (!IsValid(DA))
		{
			continue;
		}

		// 마지막 인자는 구매시에는 의미가 없지만 단지 채워주는 용도로 사용한다
		FShopItemData ItemData(
			DA->Icon.Get(),
			FText::FromName(ItemID), // 명시적 변환
			DA->Price,        // 가격이 숫자라면 AsNumber 사용
			SID.Amount,
			FGuid()
		);

		ItemList->AddItem(UShopItemItemData::Create(ItemData));
	}
}

void UShopItemListWidget::CreateSellList()
{
	auto Player = GetOwningPlayerPawn();
	if (!IsValid(Player))
	{
		return;
	}

	UInventoryComponent_V2* Inventory = Player->FindComponentByClass<UInventoryComponent_V2>();
	if (!IsValid(Inventory))
	{
		return;
	}

	const TArray<FInventorySlot>&  Slots = Inventory->GetInventorySlots();
	for (const auto& SL : Slots)
	{
		if (!SL.bUsing)
		{
			continue;
		}

		FName ItemID = SL.OwningItem.ItemID;
		const UBaseItemDataAsset* DA = Inventory->GetItemData(ItemID);
		if (!IsValid(DA))
		{
			continue;
		}

		FShopItemData ItemData(
			DA->Icon.Get(),
			FText::FromName(ItemID), // 명시적 변환
			DA->Price * 0.7,        // 가격이 숫자라면 AsNumber 사용
			SL.OwningItem.CurrentAmount,
			SL.SlotID
		);

		ItemList->AddItem(UShopItemItemData::Create(ItemData));
	}
}

void UShopItemListWidget::BuyItem()
{
	if (!mClickedData.IsValid())
	{
		return;
	}

	auto Player = GetOwningPlayerPawn();
	if (!IsValid(Player))
	{
		return;
	}

	UInventoryComponent_V2* Inventory = Player->FindComponentByClass<UInventoryComponent_V2>();
	if (!IsValid(Inventory))
	{
		return;
	}

	int32 Amount = 1;
	if (IsValid(SB_Amount))
	{
		Amount = FMath::FloorToInt(SB_Amount->GetValue());
	}

	if (mClickedData->ItemData.Amount < Amount)
	{
		mOnInteractResult.Broadcast(EShopItemInteractResultType::BUY_OUT_OF_STOCK);
		return;
	}

	float TotalPrice = mClickedData->ItemData.ItemPrice* Amount;
	float MyGold = Inventory->GetGold();
	if (MyGold < TotalPrice)
	{
		mOnInteractResult.Broadcast(EShopItemInteractResultType::BUY_NOT_ENOUGH_GOLD);
		return;
	}

	bool bSuccess = Inventory->AddItem(*(mClickedData->ItemData.ItemName.ToString()), Amount);
	if (bSuccess)
	{
		mClickedData->AddAmount(-Amount);
		//mClickedData->ItemData.Amount -= Amount;
		mOnInteractResult.Broadcast(EShopItemInteractResultType::BUY_SUCCESS);
		Inventory->AddGold(-TotalPrice);

		if (mClickedData->ItemData.Amount <= 0)
		{
			mClickedData = nullptr;
		}


		mOnItemListChanged.Broadcast();
	}
	else
	{
		mOnInteractResult.Broadcast(EShopItemInteractResultType::BUY_NOT_ENOUGH_SPACE);
		return;
	}
}

void UShopItemListWidget::SellItem()
{
	if (!mClickedData.IsValid())
	{
		return;
	}

	auto Player = GetOwningPlayerPawn();
	if (!IsValid(Player))
	{
		return;
	}

	UInventoryComponent_V2* Inventory = Player->FindComponentByClass<UInventoryComponent_V2>();
	if (!IsValid(Inventory))
	{
		return;
	}

	int32 Amount = 1;
	if (IsValid(SB_Amount))
	{
		Amount = FMath::FloorToInt(SB_Amount->GetValue());
	}

	if (mClickedData->ItemData.Amount < Amount)
	{
		mOnInteractResult.Broadcast(EShopItemInteractResultType::SELL_SUCCESS);
		return;
	}

	if (Inventory->SellItem(mClickedData->ItemData.SlotID, Amount))
	{
		if (!ItemList) return;

		// 1. 기존 데이터 및 시각적 요소 모두 삭제
		if (mClickedData->ItemData.Amount - Amount <= 0)
		{
			mClickedData = nullptr;
		}

		// 2. 다시 데이터를 채우는 로직 실행 (예: 기존에 만든 CreateBuyList 등 호출)
		RefreshItemList();

		mOnInteractResult.Broadcast(EShopItemInteractResultType::SELL_SUCCESS);
	}
}

void UShopItemListWidget::Callback_ItemEntityClick(UObject* Item)
{
	UShopItemItemData* ClickedData = Cast<UShopItemItemData>(Item);
	if (!IsValid(ClickedData))
	{
		return;
	}

	if (mClickedData.Get() == ClickedData)
	{
		return;
	}

	mClickedData = ClickedData;
	//ItemList->SetSelectedItem(Item);

	// 2. 상호작용 버튼(Btn_Interact) 활성화 등의 로직
	if (Btn_Interact)
	{
		Btn_Interact->SetIsEnabled(true);
	}
}

void UShopItemListWidget::Callback_ClickInteract()
{
	switch (mShopItemListType)
	{
	case EShopItemListType::Sell:
		SellItem();
		break;
	case EShopItemListType::Buy:
		BuyItem();
		break;
	}
}

void UShopItemListWidget::Callback_ClickShopMain()
{
	if (mGoShopMainFunc)
	{
		mGoShopMainFunc();
	}
}


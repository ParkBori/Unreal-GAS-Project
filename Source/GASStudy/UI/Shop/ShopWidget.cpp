// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopWidget.h"
#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"
#include "UI/Shop/ShopItemListWidget.h"
#include "Component/InventoryComponent_V2.h"

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Buy))
	{
		Btn_Buy->OnClicked().AddUObject(this, &UShopWidget::Callback_ClickBuy);
	}

	if (IsValid(Btn_Sell))
	{
		Btn_Sell->OnClicked().AddUObject(this, &UShopWidget::Callback_ClickSell);
	}

	if (IsValid(Btn_Exit))
	{
		Btn_Exit->OnClicked().AddUObject(this, &UShopWidget::Callback_ClickExit);
	}


	auto Player = GetOwningPlayerPawn();
	if (!IsValid(Player))
	{
		return;
	}

	UInventoryComponent_V2* Inventory = Player->FindComponentByClass<UInventoryComponent_V2>();
	if (IsValid(Inventory))
	{
		Inventory->mOnGoldChanged.AddUObject(this, &UShopWidget::Callback_ChangedMoney);
	
		if (IsValid(TB_Gold))
		{
			TB_Gold->SetText(FText::AsNumber(Inventory->GetGold()));
		}
	}
}

void UShopWidget::Init(FName NPCName)
{
	mNPCName = NPCName;

	if (IsValid(BuyList))
	{
		BuyList->Init(mNPCName, EShopItemListType::Buy);
		BuyList->mOnInteractResult.AddUObject(this, &UShopWidget::Callback_ItemListInteract);
		BuyList->mGoShopMainFunc = [this]() { ChangeShopMode(EShopModeType::None); };

		if (IsValid(SellList))
		{
			BuyList->mOnItemListChanged.AddUObject(SellList, &UShopItemListWidget::RefreshItemList);
		}
	}

	if (IsValid(SellList))
	{
		SellList->Init(mNPCName, EShopItemListType::Sell);
		SellList->mOnInteractResult.AddUObject(this, &UShopWidget::Callback_ItemListInteract);
		SellList->mGoShopMainFunc = [this]() { ChangeShopMode(EShopModeType::None); };
	}
}


void UShopWidget::Callback_ItemListInteract(EShopItemInteractResultType ResultType)
{
	if (!IsValid(TB_NPCScript))
	{
		return;
	}

	TB_NPCScript->SetVisibility(ESlateVisibility::HitTestInvisible);
	TB_NPCScript->SetText(GetInteractResultMessage(ResultType));

	auto World = GetOwningPlayer()->GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(mNPCScirptCollapsTimerHandle);
		World->GetTimerManager().SetTimer(mNPCScirptCollapsTimerHandle, FTimerDelegate::CreateUObject(this, &UShopWidget::Callback_NPCScirptCollapsTimerHandle), 3.f, false);
	}
}

void UShopWidget::ChangeShopMode(EShopModeType NewMode)
{
	switch (NewMode)
	{
	case EShopModeType::None:
		CAS->SetActiveWidget(SB_Selector);
		break;
	case EShopModeType::Buy:
		CAS->SetActiveWidget(BuyList);
		break;
	case EShopModeType::Sell:
		CAS->SetActiveWidget(SellList);
		break;
	}
}

void UShopWidget::Callback_ClickBuy()
{
	ChangeShopMode(EShopModeType::Buy);
}

void UShopWidget::Callback_ClickSell()
{
	ChangeShopMode(EShopModeType::Sell);
}

void UShopWidget::Callback_ClickExit()
{
	RemoveFromParent();
}

void UShopWidget::Callback_NPCScirptCollapsTimerHandle()
{
	TB_NPCScript->SetVisibility(ESlateVisibility::Collapsed);
}

void UShopWidget::Callback_ChangedMoney(float NewValue)
{
	if (!IsValid(TB_Gold))
	{
		return;
	}

	TB_Gold->SetText(FText::AsNumber(NewValue));
}


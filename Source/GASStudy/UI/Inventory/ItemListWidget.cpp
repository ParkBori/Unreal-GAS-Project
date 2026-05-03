// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemListWidget.h"
#include "Component/InventoryComponent_V2.h"
#include "UI/Inventory/ItemSlotWidget.h"

void UItemListWidget::Init(UInventoryComponent_V2* Comp)
{
	mInventoryComponent = Comp;
	mInventoryComponent->mOnInventoryUpdated.RemoveAll(this);
	mInventoryComponent->mOnInventoryUpdated.AddDynamic(this, &UItemListWidget::UpdateItemList);
}

//void UItemListWidget::NativeConstruct()
//{
//	if (mInventoryComponent.IsValid())
//	{
//		for (auto& slot : mSlots)
//		{
//			slot->Init(mInventoryComponent.Get());
//		}
//	}
//}

void UItemListWidget::UpdateItemList(const TArray<FInventoryUISnapshot>& Snapshot)
{
	// 인벤토리 슬롯 개수와 스냅샷 개수가 다르면 데이터가 깨지거나, 인벤토리 슬롯 설정이 잘못됨
	if (mSlots.Num() != Snapshot.Num())
	{
		return;
	}

	for (int i = 0; i < Snapshot.Num(); ++i)
	{
		mSlots[i]->Update(Snapshot[i]);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/QuickSlotWidget.h"
#include "Component/InventoryComponent_V2.h"

#include "UI/Inventory/ItemSlotWidget.h"



void UQuickSlotWidget::Init(UInventoryComponent_V2* InventoryComponent)
{
	mInventoryComponent = InventoryComponent;
	mInventoryComponent->mOnQuickSlotUpdated.RemoveAll(this);
	mInventoryComponent->mOnQuickSlotUpdated.AddDynamic(this, &UQuickSlotWidget::UpdateItemList);
}


void UQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if (mInventoryComponent.IsValid())
	//{
	//	for (auto& slot : mSlots)
	//	{
	//		slot->Init(mInventoryComponent.Get());
	//	}
	//}
}

void UQuickSlotWidget::UpdateItemList(const TArray<FInventoryUISnapshot>& Snapshot)
{
	// 인벤토리 슬롯 개수와 스냅샷 개수가 다르면 치명적인 에러

	for (int i = 0; i < Snapshot.Num(); ++i)
	{
		mSlots[i]->Update(Snapshot[i]);
	}
}
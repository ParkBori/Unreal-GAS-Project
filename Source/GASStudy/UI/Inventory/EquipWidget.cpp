// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/EquipWidget.h"
#include "Component/EquipComponent.h"
#include "UI/Inventory/ItemSlotWidget.h"


void UEquipWidget::Init(UEquipComponent* EquipComponent)
{
	mEquipComponent = EquipComponent;
	mEquipComponent->mOnEquipSlotsUpdated.RemoveAll(this);
	mEquipComponent->mOnEquipSlotsUpdated.AddUObject(this, &UEquipWidget::UpdateItemList);
}

void UEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEquipWidget::UpdateItemList(const TArray<FInventoryUISnapshot>& Snapshot)
{
	// 인벤토리 슬롯 개수와 스냅샷 개수가 다르면 치명적인 에러

	for (int i = 0; i < Snapshot.Num(); ++i)
	{
		mSlots[i]->Update(Snapshot[i]);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Component/InventoryComponent_V2.h"
#include "Component/EquipComponent.h"

#include "UI/Inventory/ItemListWidget.h"
#include "UI/Inventory/QuickSlotWidget.h"
#include "UI/Inventory/EquipWidget.h"
#include "UI/ToolTip/DefaultToolTipWidget.h"

#include "UI/Inventory/ItemSlotWidget.h"
#include "UI/Inventory/StatWidget.h"

#include "Subsystem/ItemSubsystem.h"

#include "AbilitySystemComponent.h"

void UInventoryWidget::Init(UInventoryComponent_V2* InventoryComponent, UEquipComponent* EquipComponent, UAbilitySystemComponent* AbilitySystemComponent)
{
	check(InventoryComponent);
	check(AbilitySystemComponent);

	mInventoryComponent = InventoryComponent;
	mEquipComponent = EquipComponent;
	mAbilitySystemComponent = AbilitySystemComponent;

	/*
	아이템 리스트 / 퀵스롯 / 장비 이쪽에서 툴팁 정보를 알아야지, 
	툴팁위치를 정확하게 맞추려면

	슬롯 위치를 
	*/

	if (IsValid(ItemList))
	{
		ItemList->Init(mInventoryComponent.Get());
	}

	if (IsValid(QuickSlot))
	{
		QuickSlot->Init(mInventoryComponent.Get());
	}

	if (IsValid(Equip))
	{
		Equip->Init(mEquipComponent.Get());
	}
	
	if (IsValid(Stat))
	{
		Stat->Init(mEquipComponent.Get(), mAbilitySystemComponent.Get());
	}


	auto* IS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (IS && ItemToolTip)
	{
		IS ->SetTooltipWidget(ItemToolTip);
	}
}

void UInventoryWidget::NativeConstruct()
{
	if (!ItemToolTip || !ItemList || !QuickSlot || !Equip)
	{
		UE_LOG(LogTemp, Error, TEXT("Fail ToolTip Binding")); 
		return;
	}

	//for (auto slot : ItemList->GetSlots())
	//{
	//	slot->mOnSlotHovered.AddDynamic(ItemToolTip, &UItemToolTipWidget::UpdatePositionFromSlot);
	//}
	//for (auto slot : QuickSlot->GetSlots())
	//{
	//	slot->mOnSlotHovered.AddDynamic(ItemToolTip, &UItemToolTipWidget::UpdatePositionFromSlot);
	//}
	//for (auto slot : Equip->GetSlots())
	//{
	//	slot->mOnSlotHovered.AddDynamic(ItemToolTip, &UItemToolTipWidget::UpdatePositionFromSlot);
	//}

	ItemToolTip->RemoveFromParent(); // BP에 배치돼 있다면 제거
	ItemToolTip->AddToViewport(100); // ZOrder 높게
	ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);

	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget IsInViewport: %d"),
		ItemToolTip->IsInViewport());
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ItemSubsystem.h"

#include "Component/InventoryComponent_V2.h"
#include "Component/EquipComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"


#include "DeveloperSettings/GameDataSettings.h"

#include "UI/ToolTip/DefaultToolTipWidget.h"

bool UItemSubsystem::RequestAddItem(AActor* Interactor, const FName& ItemID, int32 ItemAmount)
{
    if (!Interactor)
    {
        return false;
    }

    UInventoryComponent_V2* IC = Interactor->FindComponentByClass<UInventoryComponent_V2>();
    if (IC)
    {
        if (IC->AddItem(ItemID, ItemAmount))
        {
            // 3. 추가에 성공했을 때만 중앙 이벤트(델리게이트) 발생
            // mOnItemPickedUp.Broadcast(ItemID, ItemAmount);

            // 공통 사운드나 로그 처리

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool UItemSubsystem::RequestAddItems(AActor* Interactor, EAddPath AddPath, TMap<FName, int32> ItemList)
{
    if (!Interactor)
    {
        return false;
    }

    UInventoryComponent_V2* IC = Interactor->FindComponentByClass<UInventoryComponent_V2>();
    if (IC)
    {
        if (IC->AddItems(ItemList))
        {
            // 3. 추가에 성공했을 때만 중앙 이벤트(델리게이트) 발생
            // mOnItemPickedUp.Broadcast(ItemID, ItemAmount);

            // 공통 사운드나 로그 처리

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool UItemSubsystem::RequestHasItem(AActor* Interactor, const FName& ItemID, int32 ItemNeedAmount)
{
    if (!Interactor)
    {
        return false;
    }

    UInventoryComponent_V2* IC = Interactor->FindComponentByClass<UInventoryComponent_V2>();
    if (IC)
    {
        if (IC->HasItem(ItemID, ItemNeedAmount))
        {
            // 공통 로직

            return true;
        }
    }

    return false;
}

bool UItemSubsystem::RequestRemoveItem(AActor* Interactor, const FName& ItemID, int32 ItemRemoveAmount, bool bWorldSpawn)
{
    if (!Interactor)
    {
        return false;
    }

    UInventoryComponent_V2* IC = Interactor->FindComponentByClass<UInventoryComponent_V2>();
    if (IC)
    {
        if (IC->RemoveItem(ItemID, ItemRemoveAmount))
        {

        }
    }

    return false;
}

void UItemSubsystem::RequestItemMove(AActor* Client, const FSlotContext& From, const FSlotContext& To)
{
    if (!Client)
    {
        return;
    }

    UInventoryComponent_V2* IC = Client->FindComponentByClass<UInventoryComponent_V2>();
    UEquipComponent* EC = Client->FindComponentByClass<UEquipComponent>();
    if (!IC || !EC)
    {
        return;
    }

    int32 CurrentIdx = From.SlotIndex;
    int32 TargetIdx = To.SlotIndex;

    // Inventory -> Inventory
    if (From.IsInventory() && To.IsInventory())
    {
        IC->MoveInvenToInven(CurrentIdx, TargetIdx);
    }
    // Inventory -> QuickSlot
    else if (From.IsInventory() && To.IsQuick())
    {
        IC->MoveInvenToQuick(CurrentIdx, TargetIdx);
    }
    // Quick-> QuickSlot
    else if (From.IsQuick() && To.IsQuick())
    {
        IC->MoveQuickToQuick(CurrentIdx, TargetIdx);
    }
   
    // Inventory -> Equip
    else if (From.IsInventory() && To.IsEquip())
    {
        const FName& PrevEquipItemID = EC->GetEquipItemID(To.EquipSlotTag);
        const FName& InventoryItemID = IC->GetItemIDBySlotIdxFromInventory(From.SlotIndex);

        // 장착가능한 슬롯인지 확인
        if (EC->CanEquipToSlot(InventoryItemID, To.EquipSlotTag))
        {
            IC->RemoveItem(From.SlotIndex);

            if (!PrevEquipItemID.IsNone())
            {
                IC->AddItemToIndex(From.SlotIndex, PrevEquipItemID, 1);
            }
        }
    }
    // Equip -> Inventory 
    else if (From.IsEquip() && To.IsInventory())
    {
        // 장비슬롯 아이템 아이디 얻기
        const FName& EquipItemID = EC->GetEquipItemID(From.EquipSlotTag);
        
        if (EquipItemID.IsNone())
        {
            return;
        }
        // 아이템 추가
        if (IC->AddItemToIndex(To.SlotIndex, EquipItemID, 1))
        {
            // 아이템 추가 성공시 제거
            EC->UnEquipItem(From.EquipSlotTag);
        }
    }
}

void UItemSubsystem::RequestClearQuickSlot(AActor* Client, int32 QuickSlotIdx)
{
    UInventoryComponent_V2* IC = Client->FindComponentByClass<UInventoryComponent_V2>();
    if (!IC)
    {
        return;
    }

    IC->ClearQuickSlot(QuickSlotIdx);

}

void UItemSubsystem::RequestActivateSlotItemInteract(AActor* Client, const FSlotContext& CurrentSlotContext, const FGeometry& InGeometry)
{
    UInventoryComponent_V2* IC = Client->FindComponentByClass<UInventoryComponent_V2>();
    UEquipComponent* EC = Client->FindComponentByClass<UEquipComponent>();
    if (!IC || !EC)
    {
        return;
    }

    //툴팁으 이벤트를 중앙에 둬야, 후에 상점에서 아이템 구매할때도 도움이 됨 
    //
    //아이템 정보를 얻어야하고,
    //아이템 정보를 기반으로,


    FToolTipData TooltipData;
    FName ItemID;


    switch (CurrentSlotContext.SlotType)
    {
    case ESlotType::Equip:
        ItemID = EC->GetEquipItemID(CurrentSlotContext.EquipSlotTag);
        break;
    case ESlotType::Inventory:
        ItemID = IC->GetItemIDBySlotIdxFromInventory(CurrentSlotContext.SlotIndex);
        break;
    case ESlotType::Quick:
        ItemID = IC->GetItemIDBySlotIdxFromQuickSlots(CurrentSlotContext.SlotIndex);
        break;
    }

    const auto* DA = GetItemData(ItemID);
    if (!DA)
    {
        return;
    }

    TMap<FGameplayAttribute, float> ItemOptions;
    GetItemStat(Client, DA->ItemEffect, ItemOptions);

    if (ESlotType::Inventory == CurrentSlotContext.SlotType 
      &&  DA->CategoryTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Item.Equipment"))))
    {
        // 현재 스텟과 정보 비교해주기 
        EC->CompareItemStat(DA, ItemOptions);
    }
   
    TooltipData.Init(DA->DisplayName, DA->ItemScript, DA->Icon, ItemOptions);

    // 툴팁 업데이트  
    if (mToolTipWidget)
    {
        mToolTipWidget->UpdateToolTipData(TooltipData);
        mToolTipWidget->UpdatePositionFromSlot(InGeometry);
        mToolTipWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}

void UItemSubsystem::RequestDeActivateSlotItemInteract(AActor* Client, const FSlotContext& CurrentSlotContext)
{
    UEquipComponent* EC = Client->FindComponentByClass<UEquipComponent>();
    if (!EC)
    {
        return;
    }

    if (ESlotType::Equip != CurrentSlotContext.SlotType)
    {
        // 스텟 복구
        EC->RemovePreviewStat();
    }

    // 툴팁 제거
    if (mToolTipWidget)
    {
        mToolTipWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}


void UItemSubsystem::SetTooltipWidget(UItemToolTipWidget* TooltipWidget)
{
    mToolTipWidget = TooltipWidget;
}

void UItemSubsystem::GetItemStat(AActor* Client, TSubclassOf<UGameplayEffect> ItemEffect, OUT TMap<FGameplayAttribute, float>& ItemOptions)
{
    // 내가 얻을 수 있는 정보 -> GE 이펙트 (서브)클래스 정보
    TMap<FName, float> ItemInfo;

    
    auto* AbilitySystemComponent = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(Client);
    if (!ItemEffect || !AbilitySystemComponent)
    {
        return;
    }

    /*
    스펙핸들은 내부에 Spec 가지는 래핑클래스

    블루프린트로 생성한 이펙트들관련 정보는 Spec 에 존재

    아이템에 정의된 정보들을 얻으려면 Spec 내부의 데이터 정보에 접근이 필요

    */

    // 1. Spec 생성 (계산만)
    FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ItemEffect, 1.f, AbilitySystemComponent->MakeEffectContext());
    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

    // 이러한 접근도 가능하고
    // Spec->Def->Modifiers

    //const auto Attributes = Spec->GetModifiedAttribute();

    // 계산된 값을 가져오는거 같아서 패스
    //Spec->GetModifierMagnitude();


    if (nullptr == Spec)
    {
        return;
    }

    // Spec 의 Modifiers 정보를 채워주는 역할을 한다
    Spec->CalculateModifierMagnitudes();


    // GE 원본 데이터 (Attribute 정보가 여기 있음)
    const TArray<FGameplayModifierInfo>& ModInfos = Spec->Def->Modifiers;

    // 계산된 ModifierSpec (계산된 값만 있음)
    const TArray<FModifierSpec>& ModSpecs = Spec->Modifiers;

    for (int32 i = 0; i < Spec->Modifiers.Num(); ++i)
    {
        const FModifierSpec& ModSpec = ModSpecs[i];
        const FGameplayModifierInfo& ModInfo = ModInfos[i];
        FGameplayAttribute Attr = ModInfo.Attribute;

        ItemOptions.Add(ModInfo.Attribute, ModSpec.GetEvaluatedMagnitude());
    }

    return;
}

const UBaseItemDataAsset* UItemSubsystem::GetItemData(const FName& ItemID) const
{
    const FItemInfoBase* ItemInfo = UGameDataSettings::Get()->GetDataTableRow<FItemInfoBase>(EGlobalGameDataTableType::ItemData, ItemID);
    
    return ItemInfo == nullptr ? nullptr : ItemInfo->ItemData;
}

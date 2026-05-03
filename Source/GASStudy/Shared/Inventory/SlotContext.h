// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SlotContext.generated.h"

UENUM(BlueprintType)
enum class ESlotType : uint8
{
    Inventory,
    Quick,
    Equip,
};

USTRUCT(BlueprintType)
struct FSlotContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESlotType SlotType = ESlotType::Inventory;

    // Inventory / Quick
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SlotIndex = INDEX_NONE;

    // Equip
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag EquipSlotTag;

    bool IsInventory() const { return SlotType == ESlotType::Inventory; }
    bool IsQuick() const { return SlotType == ESlotType::Quick; }
    bool IsEquip() const { return SlotType == ESlotType::Equip; }
};


USTRUCT(BlueprintType)
struct FNPCShopItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount = 0;
};


USTRUCT(BlueprintType)
struct FNPCShopItemList : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNPCShopItemData> mShopItemData;
};
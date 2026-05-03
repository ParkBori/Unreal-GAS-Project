// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/Item/ItemInfo.h"
#include "Shared/Inventory/SlotContext.h"
#include "ActiveGameplayEffectHandle.h"
#include "Shared/Inventory/Snapshot.h"
#include "InventoryComponent_V2.generated.h"


// 여기 델리게이트를 UI 쪽에서 구독해서 
// 업데이트 되야하니깐.. 넘겨지는 정보가
// 현재 아이템 개수, 아이템 이미지 이 두가지만 있으면 됨 



#define INVENTORY_Y 10
#define INVENTORY_X 5
#define QUICKSLOT_SIZE 4


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotsUpdated, const TArray<FInventoryUISnapshot>&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotsUpdated, const TArray<FInventoryUISnapshot>&, Snapshot);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, float);


USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY()
	bool bUsing = false;

	FGuid SlotID = FGuid();

	UPROPERTY()
	FOwningItemInfo OwningItem;

	FInventorySlot()
	{
		SlotID = FGuid::NewGuid();
	}

	FString ToString()
	{
		FString Msg = FString::Printf(TEXT("%s"), bUsing ? TEXT("사용") : TEXT("미사용"));
		return FString::Printf(TEXT("%s %s"), *Msg, *OwningItem.ToString());
	}

	void Clear()
	{
		bUsing = false;
		OwningItem.Clear(); // 내부 데이터만 초기화
	}
};

USTRUCT(BlueprintType)
struct FQuickSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid LinkedSlotID;

	bool IsValid() const
	{
		return LinkedSlotID.IsValid();
	}

	void Clear()
	{
		LinkedSlotID.Invalidate();
	}
};



class UPlayerSaveGame;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UInventoryComponent_V2 : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent_V2();

public:
	void Init(class UAbilitySystemComponent* ASC);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FName& ItemID, const int32 Amount);

	UFUNCTION(BlueprintCallable)
	bool AddItems(TMap<FName, int32> ItemList);

	UFUNCTION(BlueprintCallable)
	bool SellItem(FGuid SlotID, int Amount);

	/**
	* 소유 아이템 리스트에서 특정 아이템 찾기
	*/
	bool HasItem(const FName& ItemID, const int32 ItemNeedAmount);

	/**
	* 소유 아이템 리스트에서 특정 아이템, 특정개수 제거
	*/
	bool RemoveItem(const FName& ItemID, const int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	bool AddItemToIndex(int32 SlotIndex, const FName& ItemID, const int Amount);

	FName GetItemIDBySlotIdxFromInventory(int32 SlotIdx) const;

	FName GetItemIDBySlotIdxFromQuickSlots(int32 SlotIdx) const;


	/**
	* 소유 아이템 리스트에서 특정 슬롯 아이템 제거
	*/
	UFUNCTION(BlueprintCallable)
	void RemoveItem(const int32 ItemIdx);
	
	UFUNCTION(BlueprintCallable)
	void MoveInvenToInven(const int32 CurrentIdx, const int32 TargetIdx);
	
	UFUNCTION(BlueprintCallable)
	void MoveInvenToQuick(const int32 CurrentIdx, const int32 TargetIdx);

	UFUNCTION(BlueprintCallable)
	void MoveQuickToQuick(const int32 CurrentIdx, const int32 TargetIdx);

	UFUNCTION(BlueprintCallable)
	void QuickSlotClear(int32 Index);

	void UpdateToolTip(bool bActivate, const FSlotContext& SlotContext);

	void ClearQuickSlot(int32 Index);

	bool CanUseQuickSlotItem(int SlotIdx, int UseAmount);

	bool UseItem(int SlotIdx, int UseAmount);

	float GetGold() const
	{
		return mGold;
	}

	void AddGold(float AddGold);

	const TArray<FInventorySlot>& GetInventorySlots() const
	{
		return mSlots;
	}

private:
	bool PreCheckCanAddItem(const FName& ItemID, int32 ItemAmount, int32 ItemMaxAmount);

	void AddItemToInventory(const FName& ItemID, int32 RemainingAmount, int MaxAmount);
	
public:
	const UBaseItemDataAsset* GetItemData(const FName& ItemID) const;
private:

	int32 FindEmptySlotIndex() const;
	
	const FInventorySlot* FindInventorySlotByID(FGuid ID) const;
	
	FInventorySlot* FindInventorySlotByID_NonConst(FGuid ID);
	
	int32 FindInventorySlotIndexByID(FGuid ID) const;
	
	void OnInventorySlotCleared(const FGuid& ClearedID);
	
	void GetItemStat(TSubclassOf<class UGameplayEffect> ItemEffect, OUT TMap<FGameplayAttribute, float>& ItemOptions);

	// Make Snapshot
	TArray<FInventoryUISnapshot> MakeInventorySlotsSnapshot() const;
	
	TArray<FInventoryUISnapshot> MakeQuickSlotsSnapshot() const;
	
	// Save 
	void MakeSaveData(UPlayerSaveGame* SaveGame) const;
	void LoadFromSaveData(const UPlayerSaveGame* SaveGame);


protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlot> mSlots;

	UPROPERTY(BlueprintReadOnly)
	TArray<FQuickSlot> mQuickSlots;
	
	// 외부에서 의존성 받기 
	TWeakObjectPtr<class UAbilitySystemComponent> mASC;

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventorySlotsUpdated mOnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotsUpdated mOnQuickSlotUpdated;

	FOnGoldChanged mOnGoldChanged;

	float mGold = 100000.0f;

	friend class USaveSubsystem;
};

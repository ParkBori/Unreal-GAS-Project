// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent_V2.h"
#include "AbilitySystemComponent.h"

#include "DeveloperSettings/InventorySystemDeveloperSettings.h"
#include "DeveloperSettings/GameDataSettings.h"


#include "Tag/GameplayTag.h"

#include "Save/Player/PlayerSaveGame.h"
#include "Shared/Macro/Log.h"

// Sets default values for this component's properties
UInventoryComponent_V2::UInventoryComponent_V2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventoryComponent_V2::Init(UAbilitySystemComponent* ASC)
{
	mASC = ASC;
}


// Called when the game starts
void UInventoryComponent_V2::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 인벤토리 크기 설정(Capacity)
	mSlots.SetNum(INVENTORY_Y * INVENTORY_X);
	mQuickSlots.SetNum(QUICKSLOT_SIZE);


}

bool UInventoryComponent_V2::PreCheckCanAddItem(const FName& ItemID, int32 ItemAmount, int32 ItemMaxAmount)
{
	check(ItemMaxAmount > 0);

	int32 FreeAmountInStack = 0;

	int32 EmptySlot = 0;

	for (const auto& Slot : mSlots)
	{
		if (!Slot.bUsing)
		{
			EmptySlot++;
			continue;
		}

		if (Slot.OwningItem.ItemID == ItemID)
		{
			FreeAmountInStack += (Slot.OwningItem.MaxItemCount - Slot.OwningItem.CurrentAmount);
		}
	}


	if (FreeAmountInStack >= ItemAmount)
	{
		return true;
	}

	int32 RemainingAmount = ItemAmount - FreeAmountInStack;
	int32 NeedSlot = FMath::DivideAndRoundUp(RemainingAmount, ItemMaxAmount);

	return EmptySlot >= NeedSlot;
}

void UInventoryComponent_V2::AddItemToInventory(const FName& ItemID, int32 RemainingAmount, int MaxAmount)
{
	if (MaxAmount > 1)
	{
		for (auto& Slot : mSlots)
		{
			// 새슬롯은 스킵
			if (!Slot.bUsing)
			{
				continue;
			}

			if (Slot.OwningItem.ItemID == ItemID)
			{
				if (Slot.OwningItem.CurrentAmount < Slot.OwningItem.MaxItemCount)
				{
					// 넣을 수 있는 남은 공간
					int32 CanInputAmount = Slot.OwningItem.MaxItemCount - Slot.OwningItem.CurrentAmount;
					// 실제로 추가할 수 있는 개수
					int32 AddAmount = FMath::Min(CanInputAmount, RemainingAmount);
					// 소유개수 업데이트
					Slot.OwningItem.CurrentAmount += AddAmount;
					// 남은 아이템 개수 업데이트
					RemainingAmount -= AddAmount;
				}
			}

			if (RemainingAmount <= 0)
			{
				break;
			}
		}
	}

	while (RemainingAmount > 0)
	{
		int32 EmptyIdx = FindEmptySlotIndex();
		if (EmptyIdx == INDEX_NONE)
		{
			break; // 이론상 PreCheck에서 걸러짐
		}

		int32 AddAmount = FMath::Min(MaxAmount, RemainingAmount);

		FInventorySlot& Slot = mSlots[EmptyIdx];
		Slot.bUsing = true;
		Slot.OwningItem = FOwningItemInfo(ItemID, AddAmount, MaxAmount);

		RemainingAmount -= AddAmount;
	}
}

bool UInventoryComponent_V2::AddItem(const FName& ItemID, const int32 Amount)
{
	const UBaseItemDataAsset* DA = GetItemData(ItemID);

	if (nullptr == DA)
	{
		MY_LOG(Warning, "아이템 정보가 없음");
		return false;
	}


	int32 MaxAmount = DA->Possessionlimit;
	bool bCanAdd = PreCheckCanAddItem(ItemID, Amount, MaxAmount);
	if (false == bCanAdd)
	{
		MY_LOG(Warning, "인벤토리 공간 부족");
		return false;
	}


	// 동일한 아이템 존재하는지 확인 
	
	// 아이템은 개수가 나뉘어 들어갈 수 있다. 
	// 예를들어 5개를 최대로 소유할수 있는 아이템이 있다.
	// 인벤토리에 이미 3개의 아이템을 소유할 경우 2개는 기존 아이템의 공간으로
	// 나머지는 2개는 새로운 슬롯으로 이동한다 

	AddItemToInventory(ItemID, Amount, MaxAmount);


	/*
	int RemainingAmount = Amount;
	if (MaxAmount > 1)
	{
		for (auto& Slot : mSlots)
		{
			// 새슬롯은 스킵
			if (!Slot.bUsing)
			{
				continue;
			}

			if (Slot.OwningItem.ItemID == ItemID)
			{
				if (Slot.OwningItem.CurrentAmount < Slot.OwningItem.MaxItemCount)
				{
					// 넣을 수 있는 남은 공간
					int32 CanInputAmount = Slot.OwningItem.MaxItemCount - Slot.OwningItem.CurrentAmount;
					// 실제로 추가할 수 있는 개수
					int32 AddAmount = FMath::Min(CanInputAmount, RemainingAmount);
					// 소유개수 업데이트
					Slot.OwningItem.CurrentAmount += AddAmount;
					// 남은 아이템 개수 업데이트
					RemainingAmount -= AddAmount;
				}
			}

			if (RemainingAmount <= 0)
			{
				break;
			}
		}
	}

	while (RemainingAmount > 0)
	{
		int32 EmptyIdx = FindEmptySlotIndex();
		if (EmptyIdx == INDEX_NONE)
		{
			break; // 이론상 PreCheck에서 걸러짐
		}

		int32 AddAmount = FMath::Min(MaxAmount, RemainingAmount);

		FInventorySlot& Slot = mSlots[EmptyIdx];
		Slot.bUsing = true;
		Slot.OwningItem = FOwningItemInfo(ItemID, AddAmount, MaxAmount);

		RemainingAmount -= AddAmount;
	}
	*/
	
	mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
	mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());
	
	return true;
}

bool UInventoryComponent_V2::AddItems(TMap<FName, int32> ItemList)
{
	if (ItemList.IsEmpty())
	{
		return false;
	}

	TMap<FName, int32> ItemMaxAmountList;

	// 아이템 프리체크
	for (const auto& pItem : ItemList)
	{
		const UBaseItemDataAsset* DA = GetItemData(pItem.Key);
		if (!DA)
		{
			MY_LOG(Warning, "아이템 정보가 없음");
			return false;
		}

		int32 MaxAmount = DA->Possessionlimit;
		if (!PreCheckCanAddItem(pItem.Key, pItem.Value, MaxAmount))
		{
			return false;
		}

		ItemMaxAmountList.Add(pItem.Key, DA->Possessionlimit);
	}

	// 아이템 추가하기 
	for (const auto& pItem : ItemList)
	{
		AddItemToInventory(pItem.Key, pItem.Value, ItemMaxAmountList[pItem.Key]);
	}

	mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
	mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());

	return true;
}

bool UInventoryComponent_V2::SellItem(FGuid SlotID, int Amount)
{
	const FInventorySlot* SL = FindInventorySlotByID(SlotID);
	if (!SL)
	{
		return false;
	}

	if (SL->OwningItem.CurrentAmount < Amount)
	{
		return false;
	}

	FName ItemID = SL->OwningItem.ItemID;
	const auto* DA = GetItemData(ItemID);
	if (!IsValid(DA))
	{
		return false;
	}

	// 골드 추가
	bool bSuccess = RemoveItem(ItemID, Amount);
	if (!bSuccess)
	{
		return false;
	}

	// 골드 추가 
	AddGold(Amount * (DA->Price * 0.7));

	return true;
}

bool UInventoryComponent_V2::HasItem(const FName& ItemID, const int32 ItemNeedAmount)
{
	int OwningCount = 0;

	for (const auto& Item : mSlots)
	{
		if (Item.OwningItem.ItemID == ItemID)
		{
			OwningCount += Item.OwningItem.CurrentAmount;
			if (OwningCount >= ItemNeedAmount)
			{
				return true;
			}
		}
	}

	return false;
}

bool UInventoryComponent_V2::RemoveItem(const FName& ItemID, const int32 Amount)
{
	if (!HasItem(ItemID, Amount))
	{
		return false;
	}

	// 여기에 도달했다는거는 아이템 개수가 있다는 것

	// 남은 제거 수량
	int LeftAmount = Amount;
	for (auto& Item : mSlots)
	{
		if (Item.OwningItem.ItemID == ItemID)
		{
			int32 RemoveAmount = FMath::Min(Item.OwningItem.CurrentAmount, Amount);
			Item.OwningItem.CurrentAmount -= RemoveAmount;
			if (Item.OwningItem.CurrentAmount == 0)
			{
				Item.Clear();
			}

			LeftAmount -= RemoveAmount;
			if (LeftAmount == 0)
			{
				break;
			}
		}
	}

	mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
	mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());

	return true;
}

bool UInventoryComponent_V2::AddItemToIndex(int32 SlotIndex, const FName& ItemID, const int Amount)
{
	// 유효한 인덱스 + 빈슬롯
	bool bSuccess = false;
	if (mSlots.IsValidIndex(SlotIndex) && !mSlots[SlotIndex].bUsing)
	{
		const auto* DA = GetItemData(ItemID);
		if (nullptr == DA)
		{
			MY_LOG(Error, "존재하지 않는 아이템");
			return false;;
		}
		mSlots[SlotIndex].OwningItem = FOwningItemInfo(ItemID, Amount, DA->Possessionlimit);
		mSlots[SlotIndex].bUsing = true;
		mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
		
		bSuccess = true;
	}
	else
	{
		bSuccess = AddItem(ItemID, Amount);
	}

	return bSuccess;
}

FName UInventoryComponent_V2::GetItemIDBySlotIdxFromInventory(int32 SlotIdx) const
{
	if (!mSlots.IsValidIndex(SlotIdx) || !mSlots[SlotIdx].bUsing)
	{
		return TEXT("");
	}

	return mSlots[SlotIdx].OwningItem.ItemID;
}

FName UInventoryComponent_V2::GetItemIDBySlotIdxFromQuickSlots(int32 SlotIdx) const
{
	if (mQuickSlots.IsValidIndex(SlotIdx) && mQuickSlots[SlotIdx].IsValid())
	{
		const FInventorySlot*  SL = FindInventorySlotByID(mQuickSlots[SlotIdx].LinkedSlotID);
		if (SL)
		{
			return SL->OwningItem.ItemID;
		}
	}

	return FName();
}

void UInventoryComponent_V2::RemoveItem(const int32 ItemIdx)
{
	if (!mSlots.IsValidIndex(ItemIdx))
	{
		MY_LOG(Warning, "유효하지 않는 인덱스의 아이템 제거시도");
		return;
	}

	// 슬롯아이디 캐싱
	const FGuid ClearedSlotID = mSlots[ItemIdx].SlotID;

	// 인벤토리 슬롯 비우기
	mSlots[ItemIdx].Clear();

	// 퀵슬롯 정리
	OnInventorySlotCleared(ClearedSlotID);

	// UI 갱신
	mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
}

void UInventoryComponent_V2::MoveInvenToInven(const int32 CurrentIdx, const int32 TargetIdx)
{
	if (mSlots.IsValidIndex(CurrentIdx) && mSlots[CurrentIdx].bUsing && mSlots.IsValidIndex(TargetIdx))
	{
		UE_LOG(LogTemp, Warning, TEXT("변경전"));
		UE_LOG(LogTemp, Warning, TEXT("\n%s\n%s"), *mSlots[CurrentIdx].ToString(), *mSlots[TargetIdx].ToString());
		mSlots.Swap(CurrentIdx, TargetIdx);
		UE_LOG(LogTemp, Warning, TEXT("변경후"));
		UE_LOG(LogTemp, Warning, TEXT("\n%s\n%s"), *mSlots[CurrentIdx].ToString(), *mSlots[TargetIdx].ToString());

		mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
	}
}

void UInventoryComponent_V2::MoveInvenToQuick(const int32 CurrentIdx, const int32 TargetIdx)
{
	// 실제 아이템이 옮겨지진 않고, 해당 슬롯이 인벤토리의 아이템 정보를 참조하게 된다
	if (mSlots.IsValidIndex(CurrentIdx) && mSlots[CurrentIdx].bUsing && mQuickSlots.IsValidIndex(TargetIdx))
	{

		MY_LOG(Warning, "");

		if (!mSlots[CurrentIdx].bUsing)
		{
			return;
		}

		mQuickSlots[TargetIdx].LinkedSlotID = mSlots[CurrentIdx].SlotID;
		mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());
	}
}

void UInventoryComponent_V2::MoveQuickToQuick(const int32 CurrentIdx, const int32 TargetIdx)
{
	// 실제 아이템이 옮겨지진 않고, 해당 슬롯이 인벤토리의 아이템 정보를 참조하게 된다
	if (mQuickSlots.IsValidIndex(CurrentIdx) && mQuickSlots.IsValidIndex(TargetIdx))
	{
		mQuickSlots.Swap(CurrentIdx, TargetIdx);
		mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());
	}
}

void UInventoryComponent_V2::QuickSlotClear(int32 Index)
{
	if (mQuickSlots.IsValidIndex(Index))
	{
		mQuickSlots[Index].Clear();
	}
}

void UInventoryComponent_V2::UpdateToolTip(bool bActivate, const FSlotContext& SlotContext)
{
	return;


	//if (SlotContext.SlotIndex == INDEX_NONE)
	//{
	//	return;
	//}

	//// 인벤토리 호버 -> 인벤토리에서 정보 가져오기
	//// 퀵슬롯 -> 링크된 인벤토리 정보로부터
	//// 장비슬롯 -> 장비슬롯에서 가져오기

	//FOwningItemInfo ItemInfo;
	//FToolTipData TooltipInfo;

	//if (bActivate)
	//{
	//	switch (SlotContext.SlotType)
	//	{
	//	case ESlotType::Inventory:
	//		ItemInfo = mSlots[SlotContext.SlotIndex].OwningItem;
	//		break;
	//	case ESlotType::Quick:
	//	{
	//		const auto* SL = FindInventorySlotByID(mQuickSlots[SlotContext.SlotIndex].LinkedSlotID);
	//		if (nullptr == SL)
	//		{
	//			MY_LOG(Error, "슬롯이 존재하지 않음");
	//			return;
	//		}
	//		ItemInfo = SL->OwningItem;
	//	}
	//	break;
	//	case ESlotType::Equip:
	//		//ItemInfo = mEquipmentSlots[SlotContext.SlotIndex].OwningItem;
	//		break;
	//	}

	//	if (!ItemInfo.IsValid())
	//	{
	//		MY_LOG(Error, "Can't find Slot");
	//	}


	//	const UBaseItemDataAsset* DA = GetItemData(ItemInfo.ItemID);
	//	if (nullptr == DA)
	//	{
	//		MY_LOG(Warning, "아이템 정보가 존재하지 않음");
	//		return;
	//	}

	//	// 아이템 정보 추출하기
	//	TMap<FGameplayAttribute, float> ItemOptions;
	//	GetItemStat(DA->ItemEffect, ItemOptions);

	//	// 추가적으로 포션의 경우 아이템에서 소비아이템의 경우,  



	//	if (SlotContext.IsInventory() && DA->CategoryTag.MatchesTag(TAG_ITEM_EQUIPMENT))
	//	{
	//		// 현재 스텟과 정보 비교해주기 
	//		//CompareItemStat(DA, ItemOptions);
	//	}

	//	TooltipInfo.Init(DA->DisplayName, DA->ItemScript, DA->Icon, ItemOptions);
	//}
	//else
	//{
	//	// 슬롯 호버 해제시, 스탯을 기존의 값으로 변경
	//	if (SlotContext.IsInventory())
	//	{
	//		//RestoreSnapshot();
	//	}
	//}

	//mOnToolTipUpdated.Broadcast(bActivate, TooltipInfo);
}

void UInventoryComponent_V2::ClearQuickSlot(int32 Index)
{
	if (mQuickSlots.IsValidIndex(Index))
	{
		OnInventorySlotCleared(mQuickSlots[Index].LinkedSlotID);

	}
}

bool UInventoryComponent_V2::CanUseQuickSlotItem(int SlotIdx, int UseAmount)
{
	if (!mQuickSlots.IsValidIndex(SlotIdx))
	{
		return false;
	}

	if (!mQuickSlots[SlotIdx].IsValid())
	{
		return false;
	}

	FInventorySlot* InventorySlot = FindInventorySlotByID_NonConst(mQuickSlots[SlotIdx].LinkedSlotID);
	if (nullptr == InventorySlot)
	{
		return false;
	}

	FOwningItemInfo& CurrentItem = InventorySlot->OwningItem;
	if (CurrentItem.CurrentAmount < UseAmount)
	{
		return false;
	}

	return true;
}

bool UInventoryComponent_V2::UseItem(int SlotIdx, int UseAmount)
{
	if (mQuickSlots.IsValidIndex(SlotIdx) && mQuickSlots[SlotIdx].IsValid())
	{
		 FInventorySlot* InventorySlot = FindInventorySlotByID_NonConst(mQuickSlots[SlotIdx].LinkedSlotID);
		 if (nullptr == InventorySlot)
		 {
			 MY_LOG(Error, "인벤토리슬롯 널포인트");
			 return false;
		 }
		 
		 FOwningItemInfo& CurrentItem = InventorySlot->OwningItem;

		 const auto* DA = GetItemData(CurrentItem.ItemID);
		 if (nullptr == DA)
		 {
			 MY_LOG(Error, "데이터 테이블에 존재하지 않는 아이템");
			 return false;
		 }

		 if (CurrentItem.CurrentAmount < UseAmount)
		 {
			 MY_LOG(Error, "잘못된 입력. 소유개수보다 많은 양을 사용하려고 한다");
			 return false;
		 }

		 // 아이템 효과 적용
		 bool bApplied = false;

		 if (mASC.IsValid() && DA->ItemEffect)
		 {
			 auto Spec = mASC->MakeOutgoingSpec(DA->ItemEffect, 1, mASC->MakeEffectContext());
			 if (Spec.IsValid())
			 {
				 mASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
				 bApplied = true;
			 }
		 }

		 if (!bApplied)
		 {
			 MY_LOG(Error, "아이템 사용 실패");
			 return false;
		 }
		
		 CurrentItem.CurrentAmount -= UseAmount;
		 if (CurrentItem.CurrentAmount <= 0)
		 {
			 RemoveItem(FindInventorySlotIndexByID(mQuickSlots[SlotIdx].LinkedSlotID));
		 }
		 else
		 {
			 mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
			 mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());
		 }

		 return true;
	}

	return false;
}

void UInventoryComponent_V2::AddGold(float AddGold)
{
	mGold = FMath::Clamp<float>(mGold + AddGold, 0, FLT_MAX);
	mOnGoldChanged.Broadcast(mGold);
}

TArray<FInventoryUISnapshot> UInventoryComponent_V2::MakeInventorySlotsSnapshot() const
{
	TArray<FInventoryUISnapshot> Result;

	for (const FInventorySlot& Item : mSlots)
	{
		FInventoryUISnapshot Snapshot;

		if (Item.bUsing)
		{
			const UBaseItemDataAsset* DA = GetItemData(Item.OwningItem.ItemID);
			Snapshot.bEmpty = false;
			Snapshot.ItemID = Item.OwningItem.ItemID;
			Snapshot.CurrentAmount = Item.OwningItem.CurrentAmount;
			Snapshot.Icon = DA->Icon;
		}
		else
		{
			Snapshot.bEmpty = true;
		}

		Result.Add(Snapshot);
	}

	return Result;
}

const UBaseItemDataAsset* UInventoryComponent_V2::GetItemData(const FName& ItemID) const
{
	const FItemInfoBase* ItemInfo = UGameDataSettings::Get()->GetDataTableRow<FItemInfoBase>(EGlobalGameDataTableType::ItemData, ItemID);
	return ItemInfo->ItemData;
}

int32 UInventoryComponent_V2::FindEmptySlotIndex() const
{
	for (int32 i = 0; i < mSlots.Num(); ++i)
	{
		if (!mSlots[i].bUsing)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

const FInventorySlot* UInventoryComponent_V2::FindInventorySlotByID(FGuid ID) const
{
	for (const FInventorySlot& Slot : mSlots)
	{
		if (Slot.SlotID == ID)
			return &Slot;
	}
	return nullptr;
}

FInventorySlot* UInventoryComponent_V2::FindInventorySlotByID_NonConst(FGuid ID)
{
	for (FInventorySlot& Slot : mSlots)
	{
		if (Slot.SlotID == ID)
			return &Slot;
	}
	return nullptr;
}

int32 UInventoryComponent_V2::FindInventorySlotIndexByID(FGuid ID) const
{
	for (int32 i = 0; i < mSlots.Num(); i++)
	{
		if (mSlots[i].SlotID == ID)
			return i;
	}
	return -1;
}

void UInventoryComponent_V2::OnInventorySlotCleared(const FGuid& ClearedID)
{
	for (FQuickSlot& QS : mQuickSlots)
	{
		if (QS.LinkedSlotID == ClearedID)
		{
			QS.Clear();
		}
	}

	mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());
}

TArray<FInventoryUISnapshot> UInventoryComponent_V2::MakeQuickSlotsSnapshot() const
{
	TArray<FInventoryUISnapshot> Result;
	Result.SetNum(mQuickSlots.Num());

	for (int32 i = 0; i < mQuickSlots.Num(); ++i)
	{
		const FQuickSlot& QS = mQuickSlots[i];
		FInventoryUISnapshot& Snap = Result[i];

		if (!QS.IsValid())
		{
			Snap.bEmpty = true;
			continue;
		}

		const FInventorySlot* InvSlot = FindInventorySlotByID(QS.LinkedSlotID);
		if (!InvSlot)
		{
			Snap.bEmpty = true;
			continue;
		}

		const UBaseItemDataAsset* DA = GetItemData(InvSlot->OwningItem.ItemID);
		if (!DA)
		{
			Snap.bEmpty = true;
			continue;
		}

		Snap.bEmpty = false;
		Snap.ItemID = InvSlot->OwningItem.ItemID;
		Snap.CurrentAmount = InvSlot->OwningItem.CurrentAmount;
		Snap.Icon = DA->Icon;
	}

	return Result;
}



void UInventoryComponent_V2::GetItemStat(TSubclassOf<UGameplayEffect> ItemEffect, OUT TMap<FGameplayAttribute, float>& ItemOptions)
{
	// 내가 얻을 수 있는 정보 -> GE 이펙트 (서브)클래스 정보
	TMap<FName, float> ItemInfo;

	if (!mASC.IsValid() || !ItemEffect)
	{
		MY_LOG(Error, "아이템 정보 얻기 실패")
		return;
	}


	/*
	스펙핸들은 내부에 Spec 가지는 래핑클래스

	블루프린트로 생성한 이펙트들관련 정보는 Spec 에 존재

	아이템에 정의된 정보들을 얻으려면 Spec 내부의 데이터 정보에 접근이 필요

	*/

	// 1. Spec 생성 (계산만)
	FGameplayEffectSpecHandle SpecHandle = mASC->MakeOutgoingSpec(ItemEffect, 1.f, mASC->MakeEffectContext());
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

void UInventoryComponent_V2::MakeSaveData(UPlayerSaveGame* SaveGame) const
{
	SaveGame->InventorySlotData.Empty();
	for (const FInventorySlot& Slot : mSlots)
	{
		FInventorySlotSaveData Data;
		Data.bUsing = Slot.bUsing;
		Data.SlotID = Slot.SlotID;

		if (Slot.OwningItem.IsValid())
		{
			Data.ItemID = Slot.OwningItem.ItemID;
			Data.CurrentAmount = Slot.OwningItem.CurrentAmount;
		}

		SaveGame->InventorySlotData.Add(Data);
	}

	SaveGame->QuickSlotData.Empty();
	for (const FQuickSlot& Slot : mQuickSlots)
	{
		FQuickSlotSaveData Data;
		Data.LinkedSlotID = Slot.LinkedSlotID;

		SaveGame->QuickSlotData.Add(Data);
	}

	SaveGame->Gold = mGold;
}

void UInventoryComponent_V2::LoadFromSaveData(const UPlayerSaveGame* SaveGame)
{
	if (SaveGame->InventorySlotData.IsEmpty() || SaveGame->QuickSlotData.IsEmpty())
	{
		return;
	}


	mSlots.Empty();
	mQuickSlots.Empty();

	// 인벤토리
	for (const FInventorySlotSaveData& Data : SaveGame->InventorySlotData)
	{
		FInventorySlot Slot;
		Slot.bUsing = Data.bUsing;
		Slot.SlotID = Data.SlotID;

		if (!Data.ItemID.IsNone())
		{
			const UBaseItemDataAsset* DA = GetItemData(Data.ItemID);
			if (DA)
			{
				Slot.OwningItem = FOwningItemInfo(Data.ItemID, Data.CurrentAmount, DA->Possessionlimit);
			}
		}

		mSlots.Add(Slot);
	}

	// 퀵슬롯
	for (const FQuickSlotSaveData& Data : SaveGame->QuickSlotData)
	{
		FQuickSlot Slot;
		Slot.LinkedSlotID = Data.LinkedSlotID;

		mQuickSlots.Add(Slot);
	}

	mGold = SaveGame->Gold;


	// UI 관련 델리게이트 모두 호출필요.
	mOnInventoryUpdated.Broadcast(MakeInventorySlotsSnapshot());
	mOnQuickSlotUpdated.Broadcast(MakeQuickSlotsSnapshot());
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "ItemInfo.generated.h"


/**
 * 
 */

const int32 INVENTORY_GRID_Y = 10;
const int32 INVENTORY_GRID_X = 5;

const int32 SLOTSIZE_X = 85;
const int32 SLOTSIZE_Y = 85;

UENUM(BlueprintType)
enum class EEquipmentRarity : uint8
{
	None,
	Common,
	Epic,
	Legendary,
};



USTRUCT(BlueprintType)
struct FSlotInfo
{
	GENERATED_BODY()

	UPROPERTY() // DT 로우 탐색값
	FName KeyName;

	UPROPERTY() // DT 로우 탐색값
	FName DisplayName;

	// 아이템.장비 / 아이템.방어구.전신갑옷 / 아이템.방어구.마스크
	// 아이템 고유 식별자 
	// Item.Weapon.Sword.포시작 
	UPROPERTY(EditAnywhere)
	FGameplayTag ItemTag;

	UPROPERTY()
	int32 Idx = 0;

	UPROPERTY()
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY()
	FString Script;

	UPROPERTY()
	FIntPoint GridSize = { 0, 0 };

	// 인벤토리만 사용
	UPROPERTY()
	int32 Amount = 0;

	// 장비만 사용
	UPROPERTY(EditAnywhere)
	EEquipmentRarity Rarity = EEquipmentRarity::None;

	FSlotInfo() {}

	FSlotInfo(FName InKeyName, FName InDisplayName, const FGameplayTag& InItemTag ,int32 InIdx, UTexture2D* InIcon, FString InScript, FIntPoint InGridSize, int32 InAmount, EEquipmentRarity InRarity)
		: KeyName(InKeyName)
		, DisplayName(InDisplayName)
		, ItemTag(InItemTag)
		, Idx(InIdx)
		, Icon(InIcon)
		, Script(InScript)
		, GridSize(InGridSize)
		, Amount(InAmount)
		, Rarity(InRarity)
	{

	}

	bool operator==(const FSlotInfo& Other) const
	{
		// UObject 포인터는 포인터 동일성 비교로 충분
		return KeyName == Other.KeyName
			&& DisplayName == Other.DisplayName
			&& ItemTag == Other.ItemTag
			&& Icon == Other.Icon
			&& Script == Other.Script
			&& Idx == Other.Idx
			&& GridSize == Other.GridSize
			&& Amount == Other.Amount
			&& Rarity == Other.Rarity;
	}

	bool operator!=(const FSlotInfo& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct FItemInfoBase : public FTableRowBase
{
	GENERATED_BODY()

	// 아이템 이름 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBaseItemDataAsset> ItemData;
};

UCLASS()
class GASSTUDY_API UBaseItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

public:
	// DT 에서 로우 네임으로 사용하는 정보를 데이터 내부에 소유해, 키가 사라져도 찾아서 사용할 수 있게함
	UPROPERTY(EditAnywhere)
	FName KeyName; 

	// 실제 아이템 이름  
	UPROPERTY(EditAnywhere)
	FName DisplayName;

	// 아이템.장비 / 아이템.방어구.전신갑옷 / 아이템.방어구.마스크
	UPROPERTY(EditAnywhere)
	FGameplayTag CategoryTag;

	// 아이템 고유 식별자 
	// Item.Weapon.Sword.포시작 
	UPROPERTY(EditAnywhere)
	FGameplayTag ItemTag;

	// 옵션적으로, 아이템에 추가적인 태그를 달아서 무언가 하고싶을 경우를 대비
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DynamicTags;

	// 아이템 아이콘
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere)
	bool bUseStaticMesh;

	// 아이템 메시
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseStaticMesh", EditConditionHides))
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseStaticMesh", EditConditionHides))
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	// 아이템 설명
	UPROPERTY(EditAnywhere)
	FString ItemScript;

	// 최대 소지 개수 
	UPROPERTY(EditAnywhere)
	int32 Possessionlimit;


	// ------------------- Inventory Item -------------------
	// 인벤토리 그리드 차지 칸수
	UPROPERTY(EditAnywhere)
	FIntPoint GridSize;
	// ------------------- Inventory Item -------------------
	
	// ------------------- Equipment Item -------------------
	bool bIsEquipmentItem;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsEquipmentItem", EditConditionHides))
	EEquipmentRarity Rarity;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsEquipmentItem", EditConditionHides))
	FName SocketName;

	// ------------------- Equipment Item -------------------

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayEffect> ItemEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeaponBase> SpawnWeapon;

	UPROPERTY(EditAnywhere)
	int32 Price;


	// ------------------- Quest Item -------------------
	// ------------------- Quest Item -------------------
};


// --------------- ToolTipe ---------------
USTRUCT(BlueprintType)
struct FToolTipInfo
{
	GENERATED_BODY()

	// 아이템 이름 
	FName ItemName;
	
	UPROPERTY()
	TMap<FName, float> ItemOption;

	FString ItemDescript;

	EEquipmentRarity ItemRarity;

	FVector2D Position;
	FVector2D SlotSize;

	bool operator == (const FToolTipInfo& Other)
	{
		return ItemName == Other.ItemName;
	}

	FToolTipInfo()
	{
	}

	FToolTipInfo(const FName& InItemName, const TMap<FName, float>&  InItemOption, const FString& InItemDescript, EEquipmentRarity InItemRarity, const FVector2D& InPosition, const FVector2D& InSlotSize)
		: ItemName(InItemName)
		, ItemOption(InItemOption)
		, ItemDescript(InItemDescript)
		, ItemRarity(InItemRarity)
		, Position(InPosition)
		, SlotSize(InSlotSize)
	{
	}
};

USTRUCT(BlueprintType)
struct FDiffStatusInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, float> PreViewEquipStat; // 스텟
	UPROPERTY()
	TMap<FName, float> DiffItemOption; // 아이템 차이값 

	FDiffStatusInfo()
	{

	}

	FDiffStatusInfo(const TMap<FName, float>& InPreViewEquipStat, const TMap<FName, float>& InDiffItemOption)
		: PreViewEquipStat(InPreViewEquipStat)
		, DiffItemOption(InDiffItemOption)
	{

	}

};


// 인벤토리 / 장비 컴포넌트 공동 사용 
USTRUCT(BlueprintType)
struct FToolTipData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(BlueprintReadOnly)
	FString ItemDescript;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayAttribute, float> ItemOptions;

	FToolTipData() {}

	FToolTipData(
		FName InItemId
		, int32 InCurrentAmount
		, const FString& InItemDesc
		, UTexture2D* InIcon
		, const TMap<FGameplayAttribute, float>& InItemOptions)
		: ItemName(InItemId)
		, ItemDescript(InItemDesc)
		, Icon(InIcon)
		, ItemOptions(InItemOptions)
	{

	}

	void Init(
		FName InItemId
		, const FString& InItemDesc
		, UTexture2D* InIcon
		, const TMap<FGameplayAttribute, float>& InItemOptions)
	{
		ItemName = InItemId;
		ItemDescript = InItemDesc;
		Icon = InIcon;
		ItemOptions = InItemOptions;
	}
};


USTRUCT(BlueprintType)
struct FOwningItemInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int32 CurrentAmount = 0;

	UPROPERTY()
	int32 MaxItemCount = 0;

	FOwningItemInfo() {}

	FOwningItemInfo(const FName& InItemID, int32 InCurrentAmount, int32 InMaxItemCount)
		: ItemID(InItemID)
		, CurrentAmount(InCurrentAmount)
		, MaxItemCount(InMaxItemCount)
	{

	}

	FString ToString()
	{
		return FString::Printf(TEXT("%s %d %d"), *ItemID.ToString(), CurrentAmount, MaxItemCount);
	}

	void Clear()
	{
		ItemID = NAME_None;
		CurrentAmount = 0;
		MaxItemCount = 0;
	}

	bool IsValid() const
	{
		return !ItemID.IsNone();
	}
};


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToolTipUpdated, bool/*bActivate*/, const FToolTipData& /*ToolTipInfo*/);


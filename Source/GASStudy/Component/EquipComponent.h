// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "Shared/Inventory/Snapshot.h"
#include "Shared/Item/ItemInfo.h"
#include "EquipComponent.generated.h"

class UPlayerSaveGame;
class AWeaponBase;

USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()

	UPROPERTY()
	bool bUsing = false;

	// 장비 종류 
	UPROPERTY()
	FGameplayTag EquipCategoryTag;

	// 장착된 아이템 태그 
	UPROPERTY()
	FGameplayTag EquipItemTag;

	UPROPERTY()
	FOwningItemInfo OwningItem;

	FString ToString()
	{
		FString Msg = FString::Printf(TEXT("%s"), bUsing ? TEXT("사용") : TEXT("미사용"));
		return FString::Printf(TEXT("%s %s"), *Msg, *OwningItem.ToString());
	}

	void Clear()
	{
		bUsing = false;
		EquipItemTag = FGameplayTag();
		OwningItem = FOwningItemInfo();
	}

	bool IsValid() const
	{
		return bUsing && EquipItemTag.IsValid();
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipSlotsUpdated, const TArray<FInventoryUISnapshot>& /*Snapshot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatUpdated, const FStatSnapshotMap&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChangedItem, const FGameplayTag& SlotTag, const USkeletalMesh* SKeltalMesh, const UStaticMesh* StaticMesh);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();


public:
	void Init(class UAbilitySystemComponent* ASC);

protected:
	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// 장비 장착 관련함수 
	bool CanEquipToSlot(const FName& NewItem, const FGameplayTag& SlotTag);
	
	void EquipItem(const UBaseItemDataAsset* DA, bool bLoadEquip = false);

	void UnEquipItem(const FGameplayTag& SlotTag);

	FGameplayTag GetEquipWeaponTag();

	FName GetEquipItemID(const FGameplayTag& SlotTag);

	/**
	* 같은 아이템 종류의 스텟 비교
	*/
	void CompareItemStat(const UBaseItemDataAsset* DA, const TMap<FGameplayAttribute, float>& ItemOptions);

	void RemovePreviewStat();

	AWeaponBase* GetCurrentEquipWeapon() const;

	UFUNCTION(BlueprintCallable)
	bool CheckFullEquipedState() const;

private:
	void SpawnWeaponActor(TSubclassOf<AWeaponBase> SpawWeapon);

	void DestroyWeaponActor();

	FEquipmentSlot* FindEquipmentSlotByTag(const FGameplayTag& Tag);

	void ApplyChangedItemToCharacterMesh(const FGameplayTag& SlotTag, const USkeletalMesh* SkeletalMesh, const UStaticMesh* StaticMesh);

private:
	/**
	* 아이템 데이터 함수
	*/
	const UBaseItemDataAsset* GetItemData(const FName& ItemID) const;

	/**
	* UI 에 전달한 장비 아이템 정보 생성 함수
	*/
	TArray<FInventoryUISnapshot> MakeEquipSlotsSnapshot() const;

private:
	void MakeSaveData(UPlayerSaveGame* SaveGame) const;
	void LoadFromSaveData(const UPlayerSaveGame* SaveGame);

private:
	UPROPERTY()
	TObjectPtr<AWeaponBase> mCurrentEquipWeapon;

	UPROPERTY()
	//TMap<FGameplayTag, FName> mEquipmentSlot;
	TArray<FEquipmentSlot> mEquipmentSlots;

	//UPROPERTY()
	//TMap<FGameplayTag, FEquipmentSlot> EquipmentSlots;

	/**
	* 슬롯별 적용된 아이템 이펙트 저장
	*/
	UPROPERTY()
	TMap<FGameplayTag, FActiveGameplayEffectHandle> mApplyedEffects;

	/**
	* 아이템 옵션을 차이를 저장해놓는 컨테이너
	*/
	UPROPERTY();
	TMap<FGameplayAttribute, FStatUISnapshot> mItemOptionDiff;
	
	// 외부에서 의존성 받기 
	TWeakObjectPtr<class UAbilitySystemComponent> mASC;


public:
	FOnEquipSlotsUpdated mOnEquipSlotsUpdated;
	FOnStatUpdated mOnStatUpdated;
	FOnChangedItem mOnChangedItem;

	friend class USaveSubsystem;
};

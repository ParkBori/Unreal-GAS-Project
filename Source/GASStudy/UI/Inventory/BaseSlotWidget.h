// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "Shared/Item/ItemInfo.h"
#include "BaseSlotWidget.generated.h"

class UGridInventoryWidget;

/*
드래그 중인 아이템에 필요한 정보는 무엇일까? 

1. 아이템 키이름 

2. 드래그 시작시점의 좌표 (드래그 드랍 실패시 기존의 좌표로 돌아갈수 있게 )

3. 아이템 그리드 사이즈 정보 (아이템이 현재 있는 곳을 기준으로 탐색할때 사용)
*/




UCLASS()
class USlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

protected:

public:
	FName mKeyName; // 데이터테이블의 키네임이다. (실제 아이템 이름이 아님)
	int32 mIdx;
	int32 mAmount;
	FIntPoint mGridSize;

	// 아이템 종류 / 장비종류
	// Ex) Item.Equipment.Weapon.Sword -> 
	// Ex) Item.Queset.Armor.FullArmor
	// Ex) Item.Queset.Armor.Mask
	// Ex) Item.Consumable.Potion  
	FGameplayTag mItemTag;

	FGameplayTag mSlotTag;

	// 인벤토리 -> 장비 / 퀵
	TFunction<void(int32)> RemoveItem;

	TFunction<void(int32, FName, const FGameplayTag&)> ChangeItem;

	// 장비 -> 인벤토리 (장비슬롯 비워주는 기능)
	TFunction<void()> UnEquip;

};


UCLASS()
class GASSTUDY_API UDragWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FIntPoint mSize;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SB_Size;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;
};

UCLASS()
class GASSTUDY_API UBaseSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	void ToggleToolTip(bool bEnable);
	UFUNCTION(BlueprintImplementableEvent)
	TMap<FName, float> RequestUpdateToolTip(const FName& ItemName, const FGameplayTag& SlotTag, const FVector2D& Position, const FVector2D& SlotSize);

	UFUNCTION(BlueprintImplementableEvent)
	void RequestUnHoveredEvent();

	UFUNCTION(BlueprintCallable)
	void Trash(bool bEnable)
	{

	}


public:
	void SetSlotVisibility(bool bVisible);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag mSlotTag;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDragWidget> mDragWidgetClass;

	FSlotInfo mItemSlotInfo;

};

UCLASS()
class GASSTUDY_API UInventorySlotWidget : public UBaseSlotWidget
{
	GENERATED_BODY()

protected:
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:
	void Init(const FSlotInfo& ItemSlotInfo, UGridInventoryWidget* Owner);
	void SetIdx(int32 NewIdx);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SB_Size;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Amount;

	UPROPERTY()
	TObjectPtr<UGridInventoryWidget> mOwner;
};


UCLASS()
class GASSTUDY_API UEquipmentSlotWidget : public UBaseSlotWidget
{
	GENERATED_BODY()

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:

protected:
	UFUNCTION(BlueprintCallable)
	void Callback_EquipItem(const FSlotInfo& NewEquipedItemInfo);

	// 매개변수는 의미가 없이, 뷰모델의 바인딩 형식을 맞춰주기 위함
	UFUNCTION(BlueprintCallable)
	void Callback_UnEquipItem(bool bValue);

protected:
	// UFUNCTION(BlueprintImplementableEvent)
	// bool RequestCanEquip(FName KeyName, );
	UFUNCTION(BlueprintImplementableEvent)
	void RequestEquip(FName KeyName, const FGameplayTag& SlotTag);

	UFUNCTION(BlueprintImplementableEvent)
	void RequestUnEquip(const FGameplayTag& SlotTag);
	//UFUNCTION(BlueprintImplementableEvent)
	//void RequestEquip(FName KeyName);
};


//UCLASS()
//class GASSTUDY_API UQuickSlotWidget : public UBaseSlotWidget
//{
//	GENERATED_BODY()
//
//protected:
//};

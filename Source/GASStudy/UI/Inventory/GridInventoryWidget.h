// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "Shared/Item/ItemInfo.h"
#include "GridInventoryWidget.generated.h"

/**
 * 
 */

class UInventorySlotWidget;

const int32 OFFSET_X = 5;
const int32 OFFSET_Y = 50;

UCLASS()
class GASSTUDY_API UHighlightWidget : public UUserWidget
{
	GENERATED_BODY()

	

public:
	void Activate(bool bUse, bool bCanMove, const FIntPoint& Size = FIntPoint());

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Highlight;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SB_Size;


	FLinearColor Red = { 0.881149f, 0.1 , 0.1, 0.3 };
	FLinearColor Green = { 0.1, 0.881149f, 0.1, 0.3 };
};

UCLASS()
class GASSTUDY_API UGridInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override; 
	virtual void NativePreConstruct() override;

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	UFUNCTION(BlueprintCallable)
	void AddNewItem(const FSlotInfo& NewItemInfo);
	UFUNCTION(BlueprintCallable)
	void ChangedItem(const FSlotInfo& NewItemInfo);
	UFUNCTION(BlueprintCallable)
	void SwapedItem(const FIntPoint& MovedIdx);
	UFUNCTION(BlueprintCallable)
	void MovedItem(const FIntPoint& MovedIdx);
	UFUNCTION(BlueprintCallable)
	void RemovedItem(const int32& RemovedIdx);
	// void ChangeItemAmount();
	// void RemoveItemSlot();

public:
	// 아이템의 기존위치 -> 이동된 위치
	UFUNCTION(BlueprintImplementableEvent)
	bool RequestCanMoveItem(int32 CurrentIdx, int32 TargetIdx/*const FIntPoint& CurrentGridIdx, const FIntPoint& TargetGridIdx*/);

	UFUNCTION(BlueprintImplementableEvent)
	bool RequestMoveItem(int32 CurrentIdx, int32 TargetIdx/*const FIntPoint& CurrentGridIdx, const FIntPoint& TargetGridIdx*/);

	UFUNCTION(BlueprintImplementableEvent)
	void RequestRemoveItem(int32 RemoveIdx);

	UFUNCTION(BlueprintImplementableEvent)
	void RequestChangeItem(int32 Idx, const FName& ItemName, const FGameplayTag& SlotTag);

	// 순차적으로 탐색하며 아이템 위치를 찾음
	UFUNCTION(BlueprintImplementableEvent)
	bool RequestAddItem(const FName& ItemKeyname, int32 Amount);
	
	// 특정 인덱스를 기준으로 아이템 위치가 가능한지 확이
	UFUNCTION(BlueprintImplementableEvent)
	bool RequestAddItemByIdx(const FName& ItemKeyname, int32 Idx, int32 Amount);

	UFUNCTION(BlueprintImplementableEvent)
	bool RequestCanMoveByIdx(int32 TargetIdx, const FIntPoint& GridSize);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UG_GridPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CV_ItemOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHighlightWidget> Highlight;

	UPROPERTY()
	TWeakObjectPtr<UCanvasPanelSlot> CSL_Highlight;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UInventorySlotWidget>> mInvnetorySlotArr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGridItemSlotWidget> mBackgroundSlotClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlotWidget> mItemSlotOverlayClass;
};

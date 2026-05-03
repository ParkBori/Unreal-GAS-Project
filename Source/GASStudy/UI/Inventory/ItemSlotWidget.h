// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "Shared/Inventory/SlotContext.h"
#include "ItemSlotWidget.generated.h"


UCLASS()
class UItemSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

private:
	FSlotContext mSlotContext;

	friend class UDragWidget;
	friend class UItemSlotWidget;
};

UCLASS()
class GASSTUDY_API UItemSlotDragWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;
};

/**
 * 
 */




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotHovered, const FGeometry&, SlotGeometry);

UCLASS()
class GASSTUDY_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Update(const struct FInventoryUISnapshot& Snapshot);


protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

protected:
	bool IsUsing() const
	{
		return bUsing;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FSlotContext mSlotContext;

	UPROPERTY(BlueprintReadWrite, meta=(Bindwidget))
	TObjectPtr<UImage> Img_ItemIcon;

	UPROPERTY(BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> TB_ItemAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UItemSlotDragWidget> mDragWidgetClass;

	//UPROPERTY(BlueprintReadOnly)
	//TWeakObjectPtr<class UInventoryComponent_V2> mInventoryComponent;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSlotHovered mOnSlotHovered;
};


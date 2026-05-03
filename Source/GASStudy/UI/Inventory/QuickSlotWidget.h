// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotWidget.generated.h"

/**
 * 
 */
class UInventoryComponent_V2;


UCLASS()
class GASSTUDY_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(UInventoryComponent_V2* Comp);

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void UpdateItemList(const TArray<struct FInventoryUISnapshot>& Snapshot);

public:
	const TArray<class UItemSlotWidget*>& GetSlots() const { return mSlots; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UInventoryComponent_V2> mInventoryComponent;

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<class UItemSlotWidget>> mSlots;
};

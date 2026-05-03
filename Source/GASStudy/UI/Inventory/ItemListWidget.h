// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemListWidget.generated.h"

/**
 * 
 */
class UInventoryComponent_V2;

UCLASS()
class GASSTUDY_API UItemListWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// 의존성 주입
	UFUNCTION(BlueprintCallable)
	void Init(UInventoryComponent_V2* InventoryComponent);


protected:
	//virtual void NativeConstruct() override;


protected:
	UFUNCTION(BlueprintCallable)
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

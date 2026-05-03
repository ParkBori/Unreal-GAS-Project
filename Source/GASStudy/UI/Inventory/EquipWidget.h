// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipWidget.generated.h"

/**
 * 
 */
class UEquipComponent;

UCLASS()
class GASSTUDY_API UEquipWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void Init(UEquipComponent* EquipComponent);

protected:
	virtual void NativeConstruct() override;

protected:
	void UpdateItemList(const TArray<struct FInventoryUISnapshot>& Snapshot);

public:
	const TArray<class UItemSlotWidget*>& GetSlots() const { return mSlots; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UEquipComponent> mEquipComponent;

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<class UItemSlotWidget>> mSlots;
};

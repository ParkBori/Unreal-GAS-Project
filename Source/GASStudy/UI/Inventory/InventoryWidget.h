 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UInventoryComponent_V2;
class UEquipComponent;
class UAbilitySystemComponent;

UCLASS()
class GASSTUDY_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 의존성 주입
	void Init(UInventoryComponent_V2* InventoryComponent, UEquipComponent* EquipComponent, UAbilitySystemComponent* AbilitySystemComponent);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UInventoryComponent_V2> mInventoryComponent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UEquipComponent> mEquipComponent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> mAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UItemListWidget> ItemList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UQuickSlotWidget> QuickSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UEquipWidget> Equip;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UItemToolTipWidget> ItemToolTip;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UStatWidget> Stat;
};

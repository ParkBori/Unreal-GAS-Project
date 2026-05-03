// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Pannel/BasePannel/BasePannelWidget.h"
#include "InventoryPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UInventoryPanelWidget : public UBasePannelWidget
{
	GENERATED_BODY()
	
public:
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGridInventoryWidget> GridInventory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemToolTipWidget> ItemTooltip;
};

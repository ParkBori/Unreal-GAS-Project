// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "InventorySystemDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Inventory System Settings", ToolTip = "플레이어 장비 슬롯 설정"), BlueprintType)
class GASSTUDY_API UInventorySystemDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const UInventorySystemDeveloperSettings* Get()
	{
		return GetDefault<UInventorySystemDeveloperSettings>();
	}


public:
	const TArray<FGameplayTag>& GetEquipSlotTags() const
	{
		return EquipSlotTags;
	}
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Equip Slot", meta = (AllowPrivateAccess = true))
	TArray<FGameplayTag> EquipSlotTags;
};

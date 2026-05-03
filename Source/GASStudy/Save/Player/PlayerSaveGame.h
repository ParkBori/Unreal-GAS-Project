// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Save/Player/Data/PlayerSaveData.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FInventorySlotSaveData> InventorySlotData;

	UPROPERTY()
	TArray<FQuickSlotSaveData> QuickSlotData;

	UPROPERTY()
	TArray<FEquipSlotSaveData> EquipSlotData;

	UPROPERTY()
	FQuestSaveData QuestSaveData;

	UPROPERTY()
	FTransform SavePoint;

	UPROPERTY()
	float Gold;
};

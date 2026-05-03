// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "Shared/Quest/QuestInfo.h"
#include "PlayerSaveData.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bUsing = false;

	UPROPERTY()
	FGuid SlotID = FGuid();

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int32 CurrentAmount = 0;
};

USTRUCT(BlueprintType)
struct FQuickSlotSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid LinkedSlotID;
};

USTRUCT(BlueprintType)
struct FEquipSlotSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bUsing = false;

	UPROPERTY()
	FGameplayTag EquipCategoryTag;

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int32 CurrentAmount = 0;

	FString ToString() const
	{
		return FString::Printf(
			TEXT("슬롯 사용여부 : %s / 장비카테고리테그 : %s / 아이템ID : %s"),
			(bUsing ? TEXT("사용") : TEXT("미사용")),
			*EquipCategoryTag.ToString(),
			*ItemID.ToString()
		);
	}
};

USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, FQuestProgress> CurrentProgressQuests;

	UPROPERTY()
	TSet<FName> CompletedQuests;

	UPROPERTY()
	FGameplayTagContainer GrantedTagsByQuestComplete;

	UPROPERTY()
	FName CurrentTrackingQuest;
};
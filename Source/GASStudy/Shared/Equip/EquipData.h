// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "EquipData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEquipmentVisualData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FName TargetComponentName;

    UPROPERTY(EditAnywhere)
    FName SocketName; // Attach용
};

UCLASS()
class UEquipmentVisualDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TMap<FGameplayTag, FEquipmentVisualData> SlotToVisualMap;
};

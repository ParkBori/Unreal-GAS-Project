// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboAttackData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAttackInputType : uint8
{
    LeftMouse = 0,
    RightMouse,
};

USTRUCT()
struct FComboNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    UAnimMontage* Montage = nullptr;

    UPROPERTY(EditAnywhere)
    TMap<EAttackInputType, FName> NextInfo;
};

UCLASS()
class UWeaponComboData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FName StartNodeID;

    UPROPERTY(EditAnywhere)
    TMap<FName, FComboNode> ComboMap;
};


USTRUCT(BlueprintType)
struct FDynamicCombatDataByWeapon : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UWeaponComboData> WeaponComboData = nullptr;
};


USTRUCT(BlueprintType)
struct FNoramlCombatDataByWeapon : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TObjectPtr<UAnimMontage>> NormalAttackMontageList;
};
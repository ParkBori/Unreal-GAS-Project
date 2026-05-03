// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatData.generated.h"


UENUM(BlueprintType)
enum class EHitDirection : uint8
{
    Left = 0    UMETA(DisplayName = "Left"),
    Right       UMETA(DisplayName = "Right"),
    Fwd         UMETA(DisplayName = "Fwd"),
    Back        UMETA(DisplayName = "Back"),
};


UENUM(BlueprintType)
enum class EBlockPosition : uint8
{
    Left = 0,
    Right,
};

//  
UENUM(BlueprintType)
enum class EHitDirectionArea : uint8
{
    FrontRight = 0      UMETA(DisplayName = "우상단"),
    BackRight           UMETA(DisplayName = "우하단"),
    FrontLeft           UMETA(DisplayName = "좌상단"),
    BackLeft            UMETA(DisplayName = "좌하단"),
};

UENUM(BlueprintType)
enum class ECombatInputType : uint8
{
    LeftInputAttack,
    RightInputAttack,
    Block,
};

/**
 * 공격 판정 -> 어빌리티
 */
USTRUCT(BlueprintType)
struct FAttackHitResult
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<AActor> Attacker = nullptr;
    
    UPROPERTY()
    TObjectPtr<AActor> Defender = nullptr;
    
    UPROPERTY()
    FHitResult HitResult;

    FAttackHitResult() {}

    FAttackHitResult(AActor* InAttacker, AActor* InDefender, FHitResult InHitResult)
        : Attacker(InAttacker)
        , Defender(InDefender)
        , HitResult(InHitResult)
    {

    }
};

USTRUCT()
struct FCombatInputData
{
    GENERATED_BODY()

    int32 InputID = 0;

    UPROPERTY()
    FGameplayTag InputTag; // Attack, Block, Skill1

    UPROPERTY()
    FGameplayTag SubInputTag; // Left, Right
};

USTRUCT(BlueprintType)
struct FEquipAnim : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Montage"))
    TObjectPtr<UAnimMontage> Montage = nullptr; // Attack, Block, Skill1
};


UENUM(BlueprintType)
enum class ECameraType : uint8
{
    Main,
    FinisherFront,
    FinisherBack,
};
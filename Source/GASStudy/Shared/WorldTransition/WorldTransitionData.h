// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldTransitionData.generated.h"
/**
 * 
 */


UENUM(BlueprintType)
enum class EWorldTransitionType : uint8
{
    OpenLevel,
    Teleport
};

USTRUCT(BlueprintType)
struct FTransitionRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName LevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWorldTransitionType WorldTransitionType = EWorldTransitionType::OpenLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsNewGame = false;
};


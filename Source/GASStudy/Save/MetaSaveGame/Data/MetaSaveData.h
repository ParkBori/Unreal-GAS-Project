// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetaSaveData.generated.h"

/**
 * 
 */
 // UI에 표시할 세이브 슬롯의 요약 정보입니다.
USTRUCT(BlueprintType)
struct FMetaSaveData
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
    FString SlotName;
    
    // 마지막으로 저장한 날짜와 시간
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
    FDateTime SaveDate = FDateTime();

    // 플레이 타임
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
    float PlayTime = 0.f;

    FMetaSaveData()
    {

    }

    FMetaSaveData(const FString& InSlotName)
        : SlotName(InSlotName)
    {
        SaveDate = FDateTime::Now();
    }

    bool operator == (const FMetaSaveData& Other)
    {
        return SlotName == Other.SlotName;
    }
};
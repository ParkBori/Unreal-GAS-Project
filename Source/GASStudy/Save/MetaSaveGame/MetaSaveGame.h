// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/MetaSaveGame/Data/MetaSaveData.h"
#include "MetaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UMetaSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    // 현재 존재하는 모든 세이브 슬롯의 요약 정보 리스트 (UI에서 리스트를 그릴 때 사용)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
    TArray<FMetaSaveData> mSavedSlotList;
};


// TMap -> 관리하기 편하나, Key 가 이름으로 고정되다보니, 슬롯쪽에서 이름으로 가지게 되는데
// 이게 나쁜건 아니지만 배열로하면, 그냥 인덱스로 처리가 가능하다보니 조금 고민되나

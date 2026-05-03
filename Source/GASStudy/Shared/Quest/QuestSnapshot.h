// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestInfo.h"
#include "QuestSnapshot.generated.h"

/**
 * QuestList 에서 현재 추적중인 퀘스트의 진행상황
 */


USTRUCT(BlueprintType)
struct FQuestRewardItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemIcon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemAmoun = 0;
};

USTRUCT(BlueprintType)
struct FQuestUIRewardInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Exp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FQuestRewardItemInfo> Items;
};

USTRUCT(BlueprintType)
struct FQuestUIDetailData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DetailProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FQuestUIProgressData
{
	GENERATED_BODY()

	// 해당 값을 기준으로 true -> 가로선 그어주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCurrent = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestUIDetailData> QuestUIDetailDataList;
};

/**
* 현재 활성화된  퀘스트 정보
*/
USTRUCT(BlueprintType)
struct FQuestSnapshot
{
	GENERATED_BODY()

	// 클리어시 퀘스트 이름을 반환해준다. 퀘스트 이름은 고유해야 한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestDescript;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestState;

	// 퀘스트 종류 - Main or Sub
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//EQuestType QuestType;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 CurrentStageIdx;

		// 퀘스트에서 너무 디테일한 정보는 보여주지 않을거임
	// 단, 디버깅을 위해선 표시가 필요함
	// 퀘스트 목표 진행정보
	// 정확히 작동이 되면 이부분은 표시를 UI 쪽에서 안하게
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestUIProgressData> QuestProgress;

	// 퀘스트 리워드 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestUIRewardInfo Reward;

	UPROPERTY()
	bool bIsCompletedQuest = false;

	UPROPERTY()
	bool bIsTrackingQuest = false;
};

/**
* 현재 진행중인 퀘스트, 완료된 퀘스트를 태그 + 이름 형태로 저장
*/
USTRUCT(BlueprintType)
struct FQuestListSnapshot
{
	GENERATED_BODY()

	// 현재 추적중인 퀘스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentTrackingQuest;

	// 활성화된 모든 퀘스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ActiveQuestList;

	// 완료한 퀘스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CompleteQuestList;
};

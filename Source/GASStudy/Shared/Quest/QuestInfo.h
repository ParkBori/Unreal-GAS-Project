// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestInfo.generated.h"

// 퀘스트 목표 
UENUM(BlueprintType)
enum class EQuestObjectiveType : uint8
{
	None		UMETA(DisplayName = "None"),
	Location	UMETA(DisplayName = "Location"),
	Kill		UMETA(DisplayName = "Kill"),
	Interact	UMETA(DisplayName = "Interact"),
	Collect		UMETA(DisplayName = "Collect"),
};

// 퀘스트 종류 
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	None		UMETA(DisplayName = "None"),
	MainStroy	UMETA(DisplayName = "MainStroy"),
	SubStory	UMETA(DisplayName = "SubStory"),
};

// 퀘스트 상태 
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	None						UMETA(DisplayName = "None"),				// 퀘스트 없음
	Active						UMETA(DisplayName = "Active"),				// 진행 중
	ObjectivesCompleted			UMETA(DisplayName = "ObjectivesCompleted"), // 퀘스트 목표 완료 -> 자동으로 Stage 업데이트 
	Completed					UMETA(DisplayName = "Completed"),			// 완료 -> 해당 퀘스트를 모두 완료 -> 보상프로세스로
	Failed						UMETA(DisplayName = "Failed")				// 실패
};

// 퀘스트 보상 
USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Exp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> Items;
};

// 퀘스트 목표
USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	// 퀘스트 목표 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	// 퀘스트 타입 (처치 / 수집 / 상호작용 / 위치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestObjectiveType Type = EQuestObjectiveType::None;

	// 타겟 이름  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetName;

	// 요구 수량 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredCount = 0;
};


USTRUCT(BlueprintType)
struct FQuestStage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StageName;

	// 퀘스트 목표 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestObjective> Objectives;
};

/**
UQuestDataAsset
 ├─ QuestID (GameplayTag)
 ├─ QuestName / Description
 ├─ QuestType
 ├─ PrerequisiteQuests
 ├─ Stages[]
 ├─ GrantedFlags[]
 └─ NextQuestIds[]
*/

UCLASS()
class GASSTUDY_API UQuestDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// 클리어시 퀘스트 이름을 반환해준다. 퀘스트 이름은 고유해야 한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestName;

	// 퀘스트 설명(퀘스트 전체를 통트르는 설명)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestDescript;

	// 퀘스트 종류 - Main or Sub
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType QuestType = EQuestType::None;

	// 퀘스트 수락받을 수 있는 조건  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer QuestAcceptanceConditions;

	// 퀘스트 성공시 부여되는 태그 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "본인 이름의 Tag 반드시 부여해야함"))
	FGameplayTagContainer GrantedFlags;

	// 내부에 퀘스트 요구 미션 + 보상을 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestStage> Stages;

	// 퀘스트 보상 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestReward Reward;
};

USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UQuestDataAsset> QuestDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsLastQuest", Tooltip = "마지막 퀘스트"))
	bool bIsLastQuest = false;
};

/**
FQuestProgress
 ├─ State
 └─ StageProgress
	 ├─ CurrentStageIndex
	 └─ ObjectiveProgress[Tag]

*/
USTRUCT(BlueprintType)
struct FObjectiveProgress
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Current = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Required = 0;

	bool IsCompleted() const
	{
		return Current >= Required;
	}

	void UpdateCurrentValue(int Amount)
	{
		Current = FMath::Clamp(Current + Amount, 0, Required);
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("%d / %d"), Current, Required);
	}
};

USTRUCT(BlueprintType)
struct FStageProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FObjectiveProgress> ObjectiveProgress;


	void Init(const FQuestStage& Stage)
	{
		StageName = Stage.StageName;

		for (const FQuestObjective& Objective : Stage.Objectives)
		{
			FObjectiveProgress Progress;
			Progress.Description = Objective.Description;
			Progress.Required = Objective.RequiredCount;


			ObjectiveProgress.Add(Objective.TargetName, Progress);
		}
	}


	void Update(const FName& TargetName, int Amount)
	{
		if (ObjectiveProgress.Contains(TargetName))
		{
			ObjectiveProgress[TargetName].UpdateCurrentValue(Amount);
		}
	}

	bool IsComplete() const
	{
		for (const auto& Progress : ObjectiveProgress)
		{
			if (false == Progress.Value.IsCompleted())
			{
				return false;
			}
		}

		return true;
	}
};

USTRUCT(BlueprintType)
struct FQuestProgress
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EQuestState State = EQuestState::None;

	// 현재 진행 중인 Stage Index
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentStageIndex = 0;

	// 모든 Stage 진행 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FStageProgress> StageProgressList;

	void Init(const UQuestDataAsset* QuestData)
	{
		State = EQuestState::Active;
		CurrentStageIndex = 0;

		StageProgressList.Empty();

		for (const FQuestStage& Stage : QuestData->Stages)
		{
			FStageProgress NewStage;
			NewStage.Init(Stage);
			StageProgressList.Add(NewStage);
		}
	}

	/**
	* @return 업데이트 성공 여부
	*/
	bool Update(const FName& TargetName, int Amount)
	{
		// 업데인트 실패
		if (!StageProgressList.IsValidIndex(CurrentStageIndex))
			return false;

		FStageProgress& CurrentStage = StageProgressList[CurrentStageIndex];
		CurrentStage.Update(TargetName, Amount);

		bool bIsComplete = IsStageComplete();
		if (CurrentStage.IsComplete())
		{
			CurrentStageIndex++;

			if (CurrentStageIndex >= StageProgressList.Num())
			{
				// 더이상 스테이지가 없는 경우 
				State = EQuestState::Completed;
				return true;
			}
			else
			{
				// 다음 스테이지가 존재하는 경우
				State = EQuestState::Active;
			}

			return true;
		}

		return true;
	}


	void UpdateProgress(const FName& TargetName, int Amount)
	{
		StageProgressList[CurrentStageIndex].Update(TargetName, Amount);
	}

	bool IsStageComplete() const
	{
		
		return StageProgressList[CurrentStageIndex].IsComplete();
	}

	bool IsComplete() const
	{
		return State == EQuestState::Completed;
	}
};

/**
* 퀘스트 부여하는 곳에 소유할 정보 
*/
USTRUCT(BlueprintType)
struct FQuestCommand
{
	GENERATED_BODY()

	UPROPERTY()
	FName QuestID;
};

/**
* 퀘스트 관련 Actor 들이 소유하는 정보
* ex) 특정 위치, 몬스터 처치..등
* 특정 조건이 완료되면, 해당 정보를 서브시스템 -> PlayerQuestComponent 로 전달
*/
USTRUCT(BlueprintType)
struct FQuestTargetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;
};



/**
* 퀘스트 제공자가 소유한 퀘스트 목록 
* 
* key -> Npc 이름 
* Value -> 소유한 퀘스트
*/
USTRUCT(BlueprintType)
struct FQuestTriggerData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "QuestList", Tooltip = "퀘스트 부여 목록"))
	TArray<FName> mQuestList;
};

// 이 정보를 기반으로 


// 인터렉트 -> 소유한 퀘스트 확인 -> 소유 퀘스트중에서 현재 부여할수 있는 퀘스트 확인 -> 부여 가능한 퀘스트와 연관된 다이얼로그 재생 (다이얼로그가 퀘스트 부여)
// -> 퀘스 발동 트리거 및 태그가 DA 안에 있기때문에 일일히 DA를 확인하면 엄청나게 많은 퀘스트를 확인해야할 수 있는 문제점이 생기나 지금은 그냥 유지

// 이거를 어디서 하나? -> QuestTargetCompoent 
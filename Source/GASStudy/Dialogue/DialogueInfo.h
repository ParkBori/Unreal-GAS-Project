// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Shared/Quest/QuestInfo.h"
#include "DialogueInfo.generated.h"

DECLARE_MULTICAST_DELEGATE (FOnSelectChoice);


/*
=============================================================================
    싱글 RPG용 확장형 다이얼로그 시스템 구조
    - 메인 스토리 / 서브 퀘스트 연동
    - 조건 기반 선택지 분기
    - 보상 트리거 포함
    - JSON 파싱 대응 구조
=============================================================================
*/


/*
=============================================================================
    퀘스트 상태 Enum
    - 다이얼로그 조건 체크에 사용
=============================================================================
*/


/*
=============================================================================
    다이얼로그 선택지 조건 구조체
    - 선택지 노출 여부 판단에 사용
=============================================================================
*/
USTRUCT(BlueprintType)
struct FDialogueCondition
{
    GENERATED_BODY()

    /* 요구 퀘스트 ID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName QuestID = NAME_None;

    /* 요구 퀘스트 상태 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EQuestState RequiredState = EQuestState::None;

    /* 조건 불충족 시 선택지를 숨길지 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bHideIfFailed = true;
};


/*
=============================================================================
    다이얼로그 선택지 보상 구조체
    - 선택 시 실행되는 결과
=============================================================================
*/
USTRUCT(BlueprintType)
struct FDialogueReward
{
    GENERATED_BODY()

    /* 시작할 퀘스트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName StartQuest = NAME_None;

    /* 완료 처리할 퀘스트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName CompleteQuest = NAME_None;

    /* 지급 아이템 ID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName GiveItemID = NAME_None;

    /* 부여할 GameplayTag (선택 사항) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag AddTag;
};


/*
=============================================================================
    다이얼로그 선택지 구조체
    - 실제 플레이어가 고르는 항목
=============================================================================
*/
USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    /* UI에 표시될 텍스트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ChoiceText;

    /* 선택 시 이동할 다음 노드 ID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName NextNodeID;

    /* 선택 가능 조건 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FDialogueCondition Condition;

    /* 선택 시 보상/이벤트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FDialogueReward Reward;
};

/*
=============================================================================
    다이얼로그 노드 (상태별 대답)
=============================================================================
*/
USTRUCT(BlueprintType)
struct FDialogueVariant
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FDialogueCondition Condition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogueText;
};

/*
=============================================================================
    다이얼로그 노드 (한 장면)
=============================================================================
*/
USTRUCT(BlueprintType)
struct FDialogueNode
{
    GENERATED_BODY()

    /* 고유 노드 ID (문자열 기반 권장) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName NodeID;

    /* 화자 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SpeakerName;

    /* 대사 내용 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogueText;

    /* 상태별 대사 */ 
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FDialogueVariant> Variants;

    /* 선택지 목록 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FDialogueChoice> Choices;

    /* 선택지가 없을 때 자동 이동할 노드 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName DefaultNextNodeID = NAME_None;
};





/*
=============================================================================
    전체 다이얼로그 데이터 컨테이너
    - JSON 루트 구조와 대응
=============================================================================
*/
USTRUCT(BlueprintType)
struct FDialogueData
{
    GENERATED_BODY()

    /* 모든 노드 목록 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FDialogueNode> Nodes;
};
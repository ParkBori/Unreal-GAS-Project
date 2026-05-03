// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dialogue/DialogueInfo.h"
#include "GameplayTagContainer.h"
#include "Shared/Dialogue/DialogueData.h"

#include "DialogueSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartDialogueSystem, AActor*);
DECLARE_MULTICAST_DELEGATE(FOnEndDialogueSystem);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateDialogue, const FUIDialogueData& /*UIData*/);


static const FName DialogueEndNode = TEXT("End");

/**
* 현재 캐싱하고있는 다이얼로그 정보
*/
USTRUCT(BlueprintType)
struct FDialoguePair
{
	GENERATED_BODY()

	void Clear()
	{
		Value.Nodes.Empty();
		Key = TEXT("");
	}

	// JSON 파일 이름 
	UPROPERTY()
	FName Key;

	// JSON 파싱 구조체
	UPROPERTY()
	FDialogueData Value;
};

/**
 * 
 */
UCLASS()
class GASSTUDY_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	* 다이얼로그 작동
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Activate_V2(AActor* Interactor, const FName& FileName);

	/**
	* 초이스 선택
	*/
	void SelectChoice(int32 ChoiceIdx);

	/**
	* 다음 노드
	*/
	void Next();

	/**
	* 대화 상데
	*/
	AActor* GetSpeacker() const
	{
		if (!mInteractor.IsValid())
		{
			return nullptr;
		}

		return mInteractor.Get();
	}

private:
	/**
	* 선택된 노드 재생
	*/
	void EnterNode_V2(const FName& NodeID);

	/**
	* JSON 파일을 로드하는 함수
	*/
	bool LoadDialogueFile(const FString& FileName);

	/**
	* ID로 특정 대화 노드를 가져오는 함수
	*/
	bool GetDialogueNode(FName NodeID, FDialogueNode& OutNode);

	/**
	* 진행 가능한지 확인
	*/
	bool CheckCondition(const FDialogueCondition& Condition);

private:
	/**
	* 현재 활성화된 JSON 다이얼로그
	*/
	UPROPERTY()
	FDialoguePair mCurrentDialogue;

	// JSON 파싱 구조체의 Node 캐싱
	UPROPERTY()
	TMap<FName, FDialogueNode> mDialogueNodeCachedMap; // 빠른 검색용

	/**
	* 다이얼로그 진행정보 캐싱
	* @FName - JSON 파일 이름 
	* @FName - 마지막으로 재생한 NODEID
	*/
	UPROPERTY()
	TMap<FName, FName> mDialogueList;

	/**
	* 다이얼로그를 활성화 시킨 액터(주로 NPC, Enemy) 
	*/
	UPROPERTY()
	TWeakObjectPtr<AActor> mInteractor;

	/**
	* 완전히 재생하여, 더이상 재생하지 않는 다이얼로그 목록
	*/
	UPROPERTY()
	TSet<FName> CompletedDialogues;

public:
	FOnStartDialogueSystem mOnStartDialogueSystem;
	FOnUpdateDialogue mOnUpdateDialogue;
	FOnEndDialogueSystem mOnEndDialogueSystem;

};



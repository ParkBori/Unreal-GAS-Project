// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/Quest/QuestInfo.h"
#include "Shared/Quest/QuestSnapshot.h"
#include "QuestComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatedTrackingQuest, const FQuestSnapshot&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatedQuestList, const FQuestListSnapshot&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdatedShowingQuest, const FName& /*QuestID*/, bool /*bChangedTrackingQuest*/);


class UPlayerSaveGame;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();
	
public:
	bool CanAddNewQuest(const FName& QuestID);
	bool AddNewQuest(const FName& QuestID);
	void UpdateQuestProgress(const FQuestTargetInfo& QuestTargetInfo);
	void UpdateTrackingQuest(const FName& QuestID);

	/**
	* 퀘스트 완료 -> 현재 진행중인 퀘스트 삭제
	* 퀘스트 완료 -> 완료 퀘스트에 추가
	*/
	bool CompleteQuest(const FName& QuestID, bool& bLastQuest);

	/**
	* UI 쪽에서 퀘스트 정보 요청
	*/
	void RequestQuestChangeTrackingQuest(const FName& QuestID);

	/**
	* 퀘스트 정보 요청 
	*/
	void RequestQuestData(const FName& QuestID, bool bChangeTrackingQuest);

	/**
	* QuestID 의 진행 상태를 반환
	*/
	EQuestState GetQuestState(const FName& QuestID) const;

	/**
	* 완료한 퀘스트인지 확인
	*/
	bool IsCompleteQuest(const FName& QuestID) const;

	/**
	* 현재 활성화 된 퀘스트인지 확인
	*/
	bool IsActiveQuest(const FName& QuestID) const;
private:
	/**
	* @QuestID 퀘스트 ID 
	* @bActiveQuest 현재 퀘스트가 활성화된 퀘스트인지 완료된 퀘스트인지 구분
	* @bCahngeTrackingQuest 입력으로 들어온 QuestID 로 변경할건지 여부
	*/
	void UpdateQuest(const FName& QuestID, bool bChangeTrackingQuest);

	/**
	* 퀘스트 보상 지급함수 
	* @return 아이템 추가 성공 여부
	*/
	bool GiveQuestReward(const FQuestReward& Reward);

	/**
	* 퀘스트 완료태그 부여
	* 
	* 현재 이부분 고민인게, 이 태그들을 과연 어빌리티 시스템에 적용해야하는가?
	* 지금 시점에서는 이녀석들이 플레이어 gas 로 들어가게되면.. 안됨
	* 
	* 복잡 + 플레이어 어빌리티시스템에 적용된 여러 태그중에서 이 태그들만 플래그하는게 쉽지 않을듯
	*/
	void GrantQuestFlags(const FGameplayTagContainer& TagContainer);



private:
	const FQuest* GetQuest(const FName& QuestID) const;

	// Snapshot 
	/**
	* 현재 추적중인 퀘스트가 변경 or 업데이트 
	*/
	FQuestSnapshot MakeQuestSnapshot(const FName& QuestID) const;

	/**
	* 퀘스트 리스트가 업데이트 됐을때 호출하는 함수
	* 활성 퀘스트의 변경 or 완료 퀘스트 업데이트
	*/
	FQuestListSnapshot MakeQuestListSnapshot() const;



public:
	// Save 
	void MakeSaveData(UPlayerSaveGame* SaveGame) const;
	void LoadFromSaveData(const UPlayerSaveGame* SaveGame);

private:
	/**
	* 현재 추적중이 정보 퀘스트 
	* -> HUD or 퀘스트 위치 추적등에 사용
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName mCurrentTrackingQuest;


	// 여기서 굳이 완료 퀘스트 목록을 따로 저장하기보다는
	// 음.. 고민이네 따로 저장이 필요하긴한데

	// 그러면 퀘스트 상태를...

	/**
	* 현재 진행중 퀘스 + 진행정보
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<FName, FQuestProgress> mCurrentProgressQuests;

	/**
	* 완료한 퀘스트 목록
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSet<FName> mCompletedQuests;

	UPROPERTY()
	FGameplayTagContainer mGrantedTagsByQuestComplete;

public:
	/**
	* 퀘스트 리스트가 업데이트 됐을때 호출하는 함수
	* 활성 퀘스트의 변경 or 완료 퀘스트 업데이트
	*/
	FOnUpdatedQuestList mOnUpdatedQuestList;
	/**
	* 현재 활성화중인 퀘스트 상태가 업데이트 됐을 때 
	*/
	FOnUpdatedTrackingQuest mOnUpdatedTrackingQuest;

	/**
	* 현재 보여주고 있는 퀘스트 변경
	*/
	FOnUpdatedShowingQuest mOnUpdatedShowingQuest;
};

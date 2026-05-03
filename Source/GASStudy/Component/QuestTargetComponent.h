// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/Quest/QuestInfo.h"
#include "QuestTargetComponent.generated.h"


/**
* 퀘스트 타겟 액터가 or 퀘스트 부여 액터가 소유 
* BeginPlay 시점에, QuestTargetRegistrySubsystem 에 등록됨
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UQuestTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	* mTargetInfo 기반 인터렉트 이벤트 처리
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateQuestProgress();

	/**
	* 퀘스트 부여 (다이얼로그는 따라오는 부가적인 효과임)
	*/
	UFUNCTION(BlueprintCallable)
	void GiveQuest();

	/**
	* 현재 활성화된 퀘스트가 있는지 확인 
	*/
	UFUNCTION(BlueprintCallable)
	void ActiveQuest();

	UFUNCTION(BlueprintCallable)
	void ActivateDialogue(FName DialogueName);


private:
	// Kill / Interact / Collect / Interaction
	// 퀘스트 관련 태그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FQuestTargetInfo mTargetInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Tooltip = "퀘스트 업데이트 사용 여부"))
	bool bUseQuestProgressUpdate = true;

	// 현재 활성화된 퀘스트
	FName mCurrentActivateQuest;
};



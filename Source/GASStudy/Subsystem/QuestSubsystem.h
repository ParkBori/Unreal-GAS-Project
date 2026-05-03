// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Shared/Quest/QuestInfo.h"
#include "QuestSubsystem.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddedNewQuest, const FQuest*);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateQuestProgress, const FQuest*);
DECLARE_MULTICAST_DELEGATE(FOnClearQuest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameCleared);


/**
 * 
 */

class UQuestComponent;

UCLASS()
class GASSTUDY_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Init(AActor* Player);

public:
	UFUNCTION(BlueprintCallable)
	bool IsCompletedQuest(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	bool IsActivatedQuest(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	bool CanAddQuest(const FName& QuestID);
	
	UFUNCTION(BlueprintCallable)
	bool AddNewQuest(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	void UpdateQuestProgress(const FQuestTargetInfo& QuestEvent);

	UFUNCTION(BlueprintCallable)
	void RequestQuestComplete(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	void RequestChangeTrackingQuest(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	void RequestQuestData(const FName& QuestID, bool bChangeTrackingQuest);

	EQuestState GetQuestState(const FName& QuestID) const;

private:
	UPROPERTY()
	TWeakObjectPtr<UQuestComponent> mQuestComponent;

public:
	UPROPERTY(BlueprintAssignable)
	FOnGameCleared mOnGameCleared;
};

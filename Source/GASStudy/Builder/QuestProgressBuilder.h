// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Quest/QuestInfo.h"
/**
 * 
 */
class QuestProgressBuilder
{

public:
	QuestProgressBuilder(const UQuestDataAsset* InQuestDA);

public:
	// QuestProgressBuilder& 반환하는 이유는, 계속해서 내부의 클래스의 함수를 
	// 호출시켜줘야 하기 때문!
	
	QuestProgressBuilder& FromQuestData(const UQuestDataAsset* InQuestDA);
	QuestProgressBuilder& InitializeState(EQuestState InState = EQuestState::Active);
	QuestProgressBuilder& InitializeStage(int32 StageIndex = 0);

	FQuestProgress Build() const;

private:
	const UQuestDataAsset* QuestDA = nullptr;
	FQuestProgress Progress;
};

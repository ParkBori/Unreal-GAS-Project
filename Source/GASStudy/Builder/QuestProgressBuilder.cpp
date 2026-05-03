// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/QuestProgressBuilder.h"

QuestProgressBuilder::QuestProgressBuilder(const UQuestDataAsset* InQuestDA)
{
	FromQuestData(InQuestDA);
	InitializeState();
	InitializeStage();
}

QuestProgressBuilder& QuestProgressBuilder::FromQuestData(const UQuestDataAsset* InQuestDA)
{
	QuestDA = InQuestDA;
	return *this;
}

QuestProgressBuilder& QuestProgressBuilder::InitializeState(EQuestState InState)
{
	Progress.State = InState;
	return *this;
}

QuestProgressBuilder& QuestProgressBuilder::InitializeStage(int32 StageIndex)
{
	if (!QuestDA || !QuestDA->Stages.IsValidIndex(StageIndex))
	{
		return *this;
	}

	Progress.Init(QuestDA);
	return *this;
}

FQuestProgress QuestProgressBuilder::Build() const
{
	return Progress;
}

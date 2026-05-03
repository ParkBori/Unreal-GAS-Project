// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/QuestSubsystem.h"
#include "Component/QuestComponent.h"
#include "Shared/Macro/Log.h"

#include "Subsystem/SaveSubsystem.h"

void UQuestSubsystem::Init(AActor* Player)
{
	if (!Player)
	{
		MY_LOG(Error, "");
		return;
	}

	if (UQuestComponent* QC = Player->FindComponentByClass<UQuestComponent>())
	{
		mQuestComponent = QC;
	}
}

bool UQuestSubsystem::IsCompletedQuest(const FName& QuestID)
{
	if (!mQuestComponent.IsValid())
	{
		return false;
	}

	return mQuestComponent->IsCompleteQuest(QuestID);
}

bool UQuestSubsystem::IsActivatedQuest(const FName& QuestID)
{
	if (!mQuestComponent.IsValid())
	{
		return false;
	}

	return mQuestComponent->IsActiveQuest(QuestID);
}

bool UQuestSubsystem::CanAddQuest(const FName& QuestID)
{
	if (!mQuestComponent.IsValid())
	{
		return false;
	}
	
	return mQuestComponent->CanAddNewQuest(QuestID);
}

bool UQuestSubsystem::AddNewQuest(const FName& QuestID)
{
	if (!mQuestComponent.IsValid())
	{
		return false;
	}

	return mQuestComponent->AddNewQuest(QuestID);
}

void UQuestSubsystem::UpdateQuestProgress(const FQuestTargetInfo& QuestEvent)
{
	if (!mQuestComponent.IsValid())
	{
		return;
	}
	
	mQuestComponent->UpdateQuestProgress(QuestEvent);
}

void UQuestSubsystem::RequestQuestComplete(const FName& QuestID)
{
	if (!mQuestComponent.IsValid())
	{
		return;
	}

	bool bIsLastQuest = false;
	if (mQuestComponent->CompleteQuest(QuestID, bIsLastQuest))
	{
		if (bIsLastQuest && mOnGameCleared.IsBound())
		{
			mOnGameCleared.Broadcast();
		}
	}
}

void UQuestSubsystem::RequestChangeTrackingQuest(const FName& QuestID)
{
	if (!mQuestComponent.IsValid())
	{
		return;
	}

	mQuestComponent->RequestQuestChangeTrackingQuest(QuestID);
}

void UQuestSubsystem::RequestQuestData(const FName& QuestID, bool bChangeTrackingQuest)
{
	if (!mQuestComponent.IsValid())
	{
		return;
	}

	mQuestComponent->RequestQuestData(QuestID, bChangeTrackingQuest);
}

EQuestState UQuestSubsystem::GetQuestState(const FName& QuestID) const
{
	return mQuestComponent->GetQuestState(QuestID);
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/QuestTargetRegistrySubsystem.h"

void UQuestTargetRegistrySubsystem::RegisterTarget(const FName& TargetName, AActor* Target)
{
	if (mQuestTargets.Contains(TargetName))
	{
		return;
	}

	mQuestTargets.Add(TargetName, Target);
}

void UQuestTargetRegistrySubsystem::UnregisterTarget(const FName& TargetName)
{
	mQuestTargets.Remove(TargetName);
}

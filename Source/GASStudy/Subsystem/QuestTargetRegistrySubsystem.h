// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "QuestTargetRegistrySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UQuestTargetRegistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	
public:
	void RegisterTarget(const FName& TargetName, AActor* Target);
	void UnregisterTarget(const FName& TargetTag);

private:
	TMap<FName, TWeakObjectPtr<AActor>> mQuestTargets;
};

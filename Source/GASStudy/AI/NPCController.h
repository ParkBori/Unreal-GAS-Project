// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ANPCController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPCController();

public:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	
	void RunAI();
	void StopAI();

protected:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBehaviorTree> BTAsset;

};

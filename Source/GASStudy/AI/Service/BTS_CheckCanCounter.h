// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckCanCounter.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBTS_CheckCanCounter : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_CheckCanCounter();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

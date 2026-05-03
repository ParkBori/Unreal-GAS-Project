// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CalToTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBTS_CalToTargetDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CalToTargetDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

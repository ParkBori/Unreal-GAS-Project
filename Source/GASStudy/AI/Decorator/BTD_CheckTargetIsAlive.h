// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckTargetIsAlive.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBTD_CheckTargetIsAlive : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckTargetIsAlive();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
};

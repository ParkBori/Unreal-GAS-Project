// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Strafe.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBT_Strafe : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Strafe();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere)
	bool bStrafe;
};

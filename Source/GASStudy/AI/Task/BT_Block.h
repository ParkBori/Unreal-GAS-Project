// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Block.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBT_Block : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Block();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

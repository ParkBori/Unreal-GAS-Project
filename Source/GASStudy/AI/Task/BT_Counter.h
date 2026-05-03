// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Character/CharacterInfo.h"
#include "BT_Counter.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBT_Counter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Counter();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	ECounterType CounterType = ECounterType::None;

	UPROPERTY(EditAnywhere)
	float SuccessPercent = 0.75;

};

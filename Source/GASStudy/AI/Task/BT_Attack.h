// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BT_Attack.generated.h"



/**
 * 
 */
UCLASS()
class GASSTUDY_API UBT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

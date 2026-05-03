// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BT_Strafe.h"
#include "Interface/CombatInterface.h"
#include "AIController.h"

UBT_Strafe::UBT_Strafe()
{
	NodeName = TEXT("Strafe");
}

EBTNodeResult::Type UBT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(ControlledPawn);
	if (nullptr == CombatInterface)
	{
		return EBTNodeResult::Failed;
	}

	CombatInterface->SetStrafe(bStrafe);
	return EBTNodeResult::Succeeded;
}

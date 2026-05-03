// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTD_SetGait.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/EnemyInterface.h"

UBTD_SetGait::UBTD_SetGait()
{
	bNotifyActivation = true;
}

void UBTD_SetGait::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	// Gait 교체 

	APawn* OwnerPawn = SearchData.OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(OwnerPawn))
	{
		IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(OwnerPawn);
		if (EnemyInterface)
		{
			EnemyInterface->SetEnemyGait(NewGait);
		}
	}
}

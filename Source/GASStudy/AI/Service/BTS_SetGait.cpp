// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_SetGait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"
#include "Interface/EnemyInterface.h"

UBTS_SetGait::UBTS_SetGait()
{
	NodeName = TEXT("SetGait");
}

void UBTS_SetGait::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// Gait 교체 


	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (false == IsValid(BB))
	{
		return;
	}

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn)
	{
		return;
	}

	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControlledPawn);
	if (nullptr == EnemyInterface)
	{
		return;
	}

	EGait NewGait = EGait::Jog;

	float ToTagetDistance = BB->GetValueAsFloat(BBKEY_TOTARGETDISTANCE);
	if (ToTagetDistance > ConvertToRunDistance)
	{
		NewGait = EGait::Run;
	}

	EnemyInterface->SetEnemyGait(NewGait);
}

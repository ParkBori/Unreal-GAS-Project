// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_CalToTargetDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"

UBTS_CalToTargetDistance::UBTS_CalToTargetDistance()
{
	NodeName = TEXT("BTS_CalToTargetDistance");
}

void UBTS_CalToTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (false == IsValid(BB))
	{
		return;
	}

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (false == IsValid(OwnerPawn))
	{
		return;
	}

	UObject* Targetobj = BB->GetValueAsObject(BBKEY_TARGET);
	if (nullptr == Targetobj)
	{
		return;
	}

	float Distance = OwnerPawn->GetDistanceTo(Cast<AActor>(Targetobj));
	BB->SetValueAsFloat(BBKEY_TOTARGETDISTANCE, Distance);
}

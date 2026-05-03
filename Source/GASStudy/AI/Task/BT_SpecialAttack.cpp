// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BT_SpecialAttack.h"
#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "Tag/GameplayTag.h"
#include "Interface/EnemyInterface.h"

UBT_SpecialAttack::UBT_SpecialAttack()
{
	NodeName = TEXT("BT_SpecialAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_SpecialAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn)
	{
		return EBTNodeResult::Type::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (nullptr == BB)
	{
		return EBTNodeResult::Type::Failed;
	}

	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControlledPawn);
	if (nullptr == EnemyInterface)
	{
		return EBTNodeResult::Type::Failed;
	}

	bool Success = EnemyInterface->DoSpecialAttack();
	if (Success)
	{
		return EBTNodeResult::Type::InProgress;
	}
	return EBTNodeResult::Type::Failed;
}

void UBT_SpecialAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (false == IsValid(ControlledPawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	IGameplayTagAssetInterface* TagInterface = Cast< IGameplayTagAssetInterface>(ControlledPawn);
	if (nullptr == TagInterface)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	bool AttackFinish = TagInterface->HasMatchingGameplayTag(TAG_STATE_SPECIALATTACKING);
	if (false == AttackFinish)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

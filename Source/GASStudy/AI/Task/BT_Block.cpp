// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BT_Block.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"


#include "Character/NonPlayable/Enemy/Enemy.h"
#include "Component/EnemyCombatComponent.h"

UBT_Block::UBT_Block()
{
	NodeName = TEXT("BT_Block");
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_Block::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }

    UEnemyCombatComponent* ECC = Enemy->GetCombatComponent();
    if (!ECC)
    {
        return EBTNodeResult::Failed;
    }

    bool bSuccess = ECC->ActivateBlockAbility();
    if (!bSuccess)
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}

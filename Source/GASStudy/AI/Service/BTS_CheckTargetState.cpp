// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_CheckTargetState.h"
#include "AI/AIBlackBoardData.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NonPlayable/Enemy/Enemy.h"
#include "Component/EnemyCombatComponent.h"

void UBTS_CheckTargetState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


    auto BB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
    if (!BB)
    {
        return;
    }

    AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Enemy)
    {
        return;
    }

    UEnemyCombatComponent* ECC = Enemy->GetCombatComponent();
    if (!ECC)
    {
        return;
    }

    // 🔥 플레이어 상태 Blackboard에 반영
    BB->SetValueAsBool(BBKEY_ISTARGETATTACKING, ECC->IsPlayerAttacking());
    BB->SetValueAsBool(BBKEY_ISTARGETBLOCKING, ECC->IsPlayerBlocking());
    BB->SetValueAsBool(BBKEY_ISTARGETDODGING, ECC->IsPlayerDodging());
    BB->SetValueAsBool(BBKEY_ISTARGETDRINKING, ECC->IsPlayerDrinking());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BT_Attack.h"
#include "AI/AIBlackBoardData.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Character/NonPlayable/Enemy/Enemy.h"
#include "Component/EnemyCombatComponent.h"


UBT_Attack::UBT_Attack()
{
	NodeName = TEXT("BT_Attack");
	bNotifyTick = true;
}


EBTNodeResult::Type UBT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 안전 캐스팅
    auto BB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
    if (!BB)
    {
        return EBTNodeResult::Failed;
    }

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


    float Dist = BB->GetValueAsFloat(BBKEY_TOTARGETDISTANCE);

    // 패턴 선택
    bool bSuccess = ECC->ActivateAttackAbility(Dist);
    if (!bSuccess)
    {
        return EBTNodeResult::Failed;
    }

    // Attack 종료 대기
    FDelegateHandle Handle = ECC->mOnAttackAbilityEnded.AddLambda(
        [this, &OwnerComp, ECC]()
        {
            ECC->mOnAttackAbilityEnded.RemoveAll(this); 
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        });

    return EBTNodeResult::InProgress;
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BT_Counter.h"
#include "Interface/EnemyInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"
//#include "GameplayTagAssetInterface.h"
//#include "Tag/GameplayTag.h"


UBT_Counter::UBT_Counter()
{
	NodeName = TEXT("BT_Counter");
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_Counter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn)
	{
		return EBTNodeResult::Type::Failed;
	}

	float RandPercent = FMath::RandRange(0.f, 1.0f);
	if (RandPercent <= SuccessPercent)
	{
		IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControlledPawn);
		if (EnemyInterface)
		{
			bool Success = EnemyInterface->DoCounter(CounterType);
			if (Success)
			{
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBT_Counter::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 해당 어빌리티가 다 끝나면 종료 

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(ControlledPawn);
	//if (nullptr == TagInterface)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}

	// 블럭을 기준으로 
	// 패리 성공 모션이 다 끝났을 경우 바로 공격으로 넘어가게 만들기 
	//bool TaskEndTrigger1 = TagInterface->HasMatchingGameplayTag(TAG_ANIMATION_PARRYSUCCESSMOTION);
	//bool TaskEndTrigger2 = TagInterface->HasMatchingGameplayTag(TAG_COMBAT_SUCCESSPARRY);

	// 이게 이런식이면 같은 맥락으로
	// Block 이 처음 실행되자마자 TAG_COMBAT_MELEE_BLOCK 는 존재하게되고, 
	// TAG_ANIMATION_PARRYSUCCESSMOTION 는 당연히 존재하지 않는다(실제 패링이 되었을때 생기는 태그이기 때문)
	
	// 실제 패링이 되었고, 패링이 완료된 시점에서 
	//if (TaskEndTrigger2)
	//{
	//	if (EnemyInterface)
	//	{
	//		EnemyInterface->EndCounter();
	//	}
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControlledPawn);
	if (EnemyInterface->CanFinishCounter())
	{
		EnemyInterface->EndCounter();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

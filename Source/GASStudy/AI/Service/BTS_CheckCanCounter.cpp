// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_CheckCanCounter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"
#include "AIController.h"
#include "Interface/EnemyInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Tag/GameplayTag.h"

UBTS_CheckCanCounter::UBTS_CheckCanCounter()
{
	NodeName = TEXT("CheckCanCounter");
}

void UBTS_CheckCanCounter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (nullptr == BB)
	{
		return;
	}

	UObject* Target = BB->GetValueAsObject(BBKEY_TARGET);
	if (nullptr == Target)
	{
		return;
	}

	IGameplayTagAssetInterface* TargetTags = Cast<IGameplayTagAssetInterface>(Target);
	if (nullptr == TargetTags)
	{
		return;
	}

	if (BB->GetValueAsBool(BBKEY_CANCOUNTER))
	{
		// 현재 카운터 가능상태라면, 다음 로직을 실행하지 않는다.
		return;
	}

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControllingPawn);
	if (nullptr == EnemyInterface)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Service"));



	
	// 플레이어 공격 애니메이션이 끝나야  TAG_COMBAT_MELEE_ATTACK_LIGHT 도 사라지게 되는데
	// 현재는 2 번째 데미지를 입힌 애니메이션 시점에서 해당 태그를 가지므로
	// 2번째 공격이 끝나는 시점에 CanCounter 이 켜져버림 

	// 내가 원하는건 3 번째 공격이 일어나는 시점에 바로 블럭을 해서 공격을 막거나 
	// 아니면, 블럭을 하거나 

	bool CanCounter = EnemyInterface->CheckCanCounter();

	if (CanCounter)
	{
		BB->SetValueAsBool(BBKEY_CANCOUNTER, true);
	}
}

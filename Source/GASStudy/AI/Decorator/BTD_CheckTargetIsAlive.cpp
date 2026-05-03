// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTD_CheckTargetIsAlive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"

#include "Shared/Tag/SharedTag.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


UBTD_CheckTargetIsAlive::UBTD_CheckTargetIsAlive()
{
	NodeName = TEXT("CheckTargetIsAlive");
}

bool UBTD_CheckTargetIsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);


	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (nullptr == BB)
	{
		return false;
	}

	UObject* Target = BB->GetValueAsObject(BBKEY_TARGET);
	if (nullptr == Target)
	{
		return false;
	}

	auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Cast<AActor>(Target));

	auto Tags = ASC->GetOwnedGameplayTags();

	return !Tags.HasTagExact(State::State_Dead);
}

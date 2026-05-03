// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTD_CheckTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"
#include "AIController.h"
#include "GameplayTagAssetInterface.h"

UBTD_CheckTag::UBTD_CheckTag()
{
	NodeName = TEXT("CheckTag");
}

bool UBTD_CheckTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(ControllingPawn);
	if (nullptr == GameplayTagAssetInterface)
	{
		return false;
	}

	return GameplayTagAssetInterface->HasMatchingGameplayTag(CheckTags);
}

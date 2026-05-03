// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTD_CheckGameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "AIController.h"

UBTD_CheckGameplayTags::UBTD_CheckGameplayTags()
{
	NodeName = TEXT("CheckGameplayTags");
}

bool UBTD_CheckGameplayTags::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	if (HasAnyTag)
	{
		return GameplayTagAssetInterface->HasAnyMatchingGameplayTags(CheckTags);
	}

	else if (HasAllTag)
	{
		return GameplayTagAssetInterface->HasAllMatchingGameplayTags(CheckTags);
	}

	else
	{
		return false;
	}
}

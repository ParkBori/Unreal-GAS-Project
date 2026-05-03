// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckGameplayTags.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBTD_CheckGameplayTags : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckGameplayTags();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere)
	bool HasAnyTag;
	UPROPERTY(EditAnywhere)
	bool HasAllTag;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer CheckTags;
};

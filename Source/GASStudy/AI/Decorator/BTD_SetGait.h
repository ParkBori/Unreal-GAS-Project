// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Character/CharacterInfo.h"
#include "BTD_SetGait.generated.h"

/**
 * 
 */


UCLASS()
class GASSTUDY_API UBTD_SetGait : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_SetGait();

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

protected:
	UPROPERTY(EditAnywhere)
	EGait NewGait;

	UPROPERTY(EditAnywhere)
	bool bStarfe = false;
};

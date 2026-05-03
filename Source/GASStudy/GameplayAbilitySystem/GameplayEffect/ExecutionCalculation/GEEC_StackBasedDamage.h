// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_StackBasedDamage.generated.h"

/**
 * 스택 기반 데미지
 * 스택 증가 -> 데미지 증가
 */
UCLASS()
class GASSTUDY_API UGEEC_StackBasedDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_StackBasedDamage();

public:
	/**
	* @ExecutionParams : 이펙트 발동 정보
	* @OutExecutionOutput : 어트리뷰트 수정 정보
	*/
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
	
};

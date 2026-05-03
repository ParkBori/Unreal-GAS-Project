// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_NormalDamage.generated.h"

/**
 * 일반 데미지 계산 
 * 데미지 - 방어력 -> 최종 데미지
 */
UCLASS()
class GASSTUDY_API UGEEC_NormalDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_NormalDamage();
	
public:
	/**
	* @ExecutionParams : 이펙트 발동 정보  
	* @OutExecutionOutput : 어트리뷰트 수정 정보 
	*/
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

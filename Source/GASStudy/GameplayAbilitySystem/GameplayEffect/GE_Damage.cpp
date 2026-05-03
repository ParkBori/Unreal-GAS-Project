// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/GE_Damage.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_NormalDamage.h"

UGE_Damage::UGE_Damage()
{
	// 한 프레임에 영향을 줌 
	DurationPolicy = EGameplayEffectDurationType::Instant;
}

UGE_NormalDamage::UGE_NormalDamage()
{
	FGameplayEffectExecutionDefinition Execution;
	Execution.CalculationClass = UGEEC_NormalDamage::StaticClass();
	Executions.Add(Execution);
}

UGE_TrueDamage::UGE_TrueDamage()
{
}

UGE_RadiusDamage::UGE_RadiusDamage()
{
}

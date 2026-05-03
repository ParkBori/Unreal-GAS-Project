// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_TrueDamage.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Shared/Tag/SharedTag.h"


// 1. 속성 캡처를 위한 구조체 정의
struct FTrueDamageStatics
{
	// ===== Target ASC Attributes =====
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);     // 이펙트가 적용되는 쪽

	FTrueDamageStatics()
	{
		// ===== Target ASC Capture =====
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Health, Target, false);
	}
};

static const FTrueDamageStatics& GetTrueDamageStatics()
{
	static FTrueDamageStatics DStatics;
	return DStatics;
}

UGEEC_TrueDamage::UGEEC_TrueDamage()
{
    RelevantAttributesToCapture.Add(GetTrueDamageStatics().HealthDef);
}

void UGEEC_TrueDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    FGameplayEffectContextHandle Context = Spec.GetContext();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    // ===== Attribute Capture 값 처리 =====

    float ModifierRes = 1.f;
    ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(Effect::Effect_Param_DamageRate, EvaluateParams, ModifierRes);

    float FinalDamage = ModifierRes;

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetTrueDamageStatics().HealthProperty, EGameplayModOp::AddBase, -FinalDamage));
    OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
    OutExecutionOutput.MarkStackCountHandledManually();
}

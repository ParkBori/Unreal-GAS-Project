// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_StackBasedDamage.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

#include "Shared/Tag/SharedTag.h"

struct FStackBasedDamageStatics
{
    // ===== Target ASC Attributes =====
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

    FStackBasedDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Health, Target, true);
    }
};

static FStackBasedDamageStatics& GetStackBasedCombatStatics()
{
    static FStackBasedDamageStatics Statics;
    return Statics;
}

UGEEC_StackBasedDamage::UGEEC_StackBasedDamage()
{
#if WITH_EDITORONLY_DATA
    ValidTransientAggregatorIdentifiers.AddTag(Effect::Effect_Param_DamageRate);
#endif

    RelevantAttributesToCapture.Add(GetStackBasedCombatStatics().HealthDef);
}

void UGEEC_StackBasedDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

    float FinalDamage = Spec.GetStackCount() * ModifierRes;

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetStackBasedCombatStatics().HealthProperty, EGameplayModOp::AddBase, -FinalDamage));
    OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
    OutExecutionOutput.MarkStackCountHandledManually();
}



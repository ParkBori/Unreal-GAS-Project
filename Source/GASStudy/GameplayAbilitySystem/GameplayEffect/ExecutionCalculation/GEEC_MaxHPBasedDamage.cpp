// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_MaxHPBasedDamage.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Shared/Tag/SharedTag.h"

struct FMaxHPBasedDamageStatics
{
    // ===== Target ASC Attributes =====
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

    FMaxHPBasedDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Health, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, MaxHealth, Target, true);
    }
};

static FMaxHPBasedDamageStatics& GetMaxHPBasedCombatStatics()
{
    static FMaxHPBasedDamageStatics Statics;
    return Statics;
}

UGEEC_MaxHPBasedDamage::UGEEC_MaxHPBasedDamage()
{
    RelevantAttributesToCapture.Add(GetMaxHPBasedCombatStatics().MaxHealthDef);
}

void UGEEC_MaxHPBasedDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    FGameplayEffectContextHandle Context = Spec.GetContext();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    // ===== Attribute Capture 값 처리 =====

    float MaxHP = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetMaxHPBasedCombatStatics().MaxHealthDef, EvaluateParams, MaxHP);

    float ModifierRes = 0.01f;
    ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(Effect::Effect_Param_DamageRate, EvaluateParams, ModifierRes);

    float FinalDamage = MaxHP * ModifierRes;

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetMaxHPBasedCombatStatics().HealthProperty, EGameplayModOp::AddBase, -FinalDamage));
    OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
    OutExecutionOutput.MarkStackCountHandledManually();
}

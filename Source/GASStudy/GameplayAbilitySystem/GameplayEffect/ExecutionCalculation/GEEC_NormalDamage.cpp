// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_NormalDamage.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Shared/Tag/SharedTag.h"

// 1. 속성 캡처를 위한 구조체 정의
struct FNormalDamageStatics
{
	// ===== Source ASC Attributes =====
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage); // 이펙트를 발동시킨 쪽

	// ===== Target ASC Attributes =====
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);    // 이펙트가 적용되는 쪽
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);     // 이펙트가 적용되는 쪽

	FNormalDamageStatics()
	{
		// false 옵션은 "스냅샷"을 찍지 않겠다는 뜻으로, GE가 발생하는 그 최종 순간의 실시간 스탯을 가져오겠다는 의미입니다.

		// ===== Source ASC Capture =====
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Damage, Source, true);
		// ===== Target ASC Capture =====
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Health, Target, false);
	}
};

static const FNormalDamageStatics& GetDamageStatics()
{
	static FNormalDamageStatics DStatics;
	return DStatics;
}

UGEEC_NormalDamage::UGEEC_NormalDamage()
{
	// 이 계산기에서 사용할 속성들을 리스트에 추가
	RelevantAttributesToCapture.Add(GetDamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().DamageDef);
}

void UGEEC_NormalDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// 가해자와 피해자의 태그 정보 가져오기 (필요 시)
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 실제 값 캡처
	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DamageDef, EvaluationParameters, Damage);

	float DmgCoefficient = Spec.GetSetByCallerMagnitude(Effect::Effect_Param_DmgCoefficient);
	DmgCoefficient = FMath::Max<float>(1.f, DmgCoefficient);
	Damage *= DmgCoefficient;

	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DefenseDef, EvaluationParameters, Defense);

	float FinalDamage = FMath::Max<float>(Damage - Defense, 0.f);

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
		// 조건이 만족될시 적용되는 추가 이펙트가 존재한다면,
		// 현재 이펙트 결과가 적용됐으니 조건이 있다면 확인 -> 조건 만족시 추가 이펙트 발동
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	}
}
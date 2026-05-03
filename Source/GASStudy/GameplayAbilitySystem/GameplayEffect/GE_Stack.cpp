// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/GE_Stack.h"

#include "Shared/Tag/SharedTag.h"

#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_TrueDamage.h"
#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_StackBasedDamage.h"
#include "GameplayAbilitySystem/GameplayEffect/ExecutionCalculation/GEEC_MaxHPBasedDamage.h"

#include "GameplayEffectComponents/TargetTagRequirementsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayAbilitySystem/GameplayEffect/Components/SpawnActorAttachGEComponent.h"

UGE_Stack::UGE_Stack()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Period.SetValue(1.f);

	// 게임플레이 큐가 처음 적용됐을때 한번만 작동
	bSuppressStackingCues = true;

	// 누가 이펙트를 적용시켜도, 캐스터에 상관없이 타겟은 통합으로 관리한다
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::NeverRefresh;
	StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::NeverReset;
	// 1개씩 감소
	StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::RemoveSingleStackAndRefreshDuration;
	StackLimitCount = 5;
}

UGE_DebuffStack::UGE_DebuffStack()
{
	// 타깃 태그에 특정 태그가 존재하거나 존재하지 않을 경우에만 디버프를 적용
	auto* TTRGEC = CreateDefaultSubobject<UTargetTagRequirementsGameplayEffectComponent>(TEXT("TargetTagRequirementsGameplayEffectComponent"));
	//TTRGEC->ApplicationTagRequirements.RequireTags.AddTag(State::State_Debuffable);
	GEComponents.Add(TTRGEC);
}

UGE_ScalableStack::UGE_ScalableStack()
{
	FSetByCallerFloat SetByCallerFloat;
	SetByCallerFloat.DataTag = Effect::Effect_Param;
	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);
}

UGE_ScalableDebuffStack::UGE_ScalableDebuffStack()
{
	auto* TTRGEC = CreateDefaultSubobject<UTargetTagRequirementsGameplayEffectComponent>(TEXT("TargetTagRequirementsGameplayEffectComponent"));
	//TTRGEC->ApplicationTagRequirements.RequireTags.AddTag(State::State_Debuffable);
	GEComponents.Add(TTRGEC);
}

UGE_BleedingState::UGE_BleedingState()
{
	// 타겟 부여 태그
	FInheritedTagContainer GrantedTags;
	GrantedTags.AddTag(Debuff::Effect_Debuff_Bleeding);

	// 타겟에게 부여할 태그 
	auto* TTGEC = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsGameplayEffectComponent"));
	TTGEC->SetAndApplyTargetTagChanges(GrantedTags);
	GEComponents.Add(TTGEC);

	// 상태이상 나이아가라 스폰
	auto* SAAGEC = CreateDefaultSubobject<USpawnNiagaraAttachGEComponent>(TEXT("SpawnActorAttachGEComponent"));
	FSpawnAttachedNiagaraParameters Params;
	Params.mNiagaraAsset = FSoftObjectPath(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Effect/Blood_VFX_Pack/Particles/Systems/P_Bleeding_Ribbon.P_Bleeding_Ribbon'"));
	Params.mHasToAttach = true;
	Params.mTransformType = ESpawnTransformType::Target;
	Params.mAttachType = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Params.mSocketName = TEXT("pelvis");
	Params.mAttachType.ScaleRule = EAttachmentRule::KeepWorld;
	SAAGEC->SetParameters(Params);
	GEComponents.Add(SAAGEC);

	// 스택 개수 * 2배의 데미지 적용 
	{
		// 계산 클래스 설정 
		FGameplayEffectExecutionDefinition Execution;
		Execution.CalculationClass = UGEEC_StackBasedDamage::StaticClass();
		// GEEC 내부로 전달할 데이터 생성
		FGameplayEffectExecutionScopedModifierInfo ModifierInfo;
		// 수치 처리 방식
		ModifierInfo.ModifierOp = EGameplayModOp::AddBase;
		// 값 식별 태그
		ModifierInfo.TransientAggregatorIdentifier = Effect::Effect_Param_DamageRate;
		// 임시 데이터 공간(휘발성)
		ModifierInfo.AggregatorType = EGameplayEffectScopedModifierAggregatorType::Transient;
		// 값 설정 
		ModifierInfo.ModifierMagnitude = FScalableFloat(2.f);
		// 등록
		Execution.CalculationModifiers.Add(ModifierInfo);
		Executions.Add(Execution);
	}

	// 큐 등록
	FGameplayEffectCue HitCue;
	HitCue.GameplayCueTags.AddTag(GameplayCue::GameplayCue_Debuff_Bleeding);
	HitCue.MinLevel = 1.f;
	HitCue.MaxLevel = 5.f;
	GameplayCues.Add(HitCue);

	// 5초 지속시간 고정
	DurationMagnitude = FScalableFloat(5.f);
}

UGE_BurnState::UGE_BurnState()
{
	// 타겟 부여 태그
	FInheritedTagContainer GrantedTags;
	GrantedTags.AddTag(Debuff::Effect_Debuff_Burn);

	// 타겟에게 부여할 태그 
	auto* TTGEC = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsGameplayEffectComponent"));
	TTGEC->SetAndApplyTargetTagChanges(GrantedTags);
	GEComponents.Add(TTGEC);

	// 상태이상 나이아가라 스폰
	auto* SAAGEC = CreateDefaultSubobject<USpawnNiagaraAttachGEComponent>(TEXT("SpawnNiagaraAttachGEComponent"));
	FSpawnAttachedNiagaraParameters Params;
	Params.mNiagaraAsset = FSoftObjectPath(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Effect/M5VFXVOL2/Niagara/Reference/Fireloop/3_NblowingfireB_fwd_pt.3_NblowingfireB_fwd_pt'"));
	Params.mHasToAttach = true;
	Params.mTransformType = ESpawnTransformType::Target;
	Params.mAttachType = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Params.mSocketName = TEXT("pelvis");
	Params.mAttachType.ScaleRule = EAttachmentRule::KeepWorld;
	SAAGEC->SetParameters(Params);
	GEComponents.Add(SAAGEC);

	// 데미지
	{
		// 계산 클래스 설정 
		FGameplayEffectExecutionDefinition Execution;
		Execution.CalculationClass = UGEEC_TrueDamage::StaticClass();
		// GEEC 내부로 전달할 데이터 생성
		FGameplayEffectExecutionScopedModifierInfo ModifierInfo;
		// 수치 처리 방식
		ModifierInfo.ModifierOp = EGameplayModOp::AddBase;
		// 값 식별 태그
		ModifierInfo.TransientAggregatorIdentifier = Effect::Effect_Param_DamageRate;
		// 임시 데이터 공간(휘발성)
		ModifierInfo.AggregatorType = EGameplayEffectScopedModifierAggregatorType::Transient;
		// 값 설정 
		ModifierInfo.ModifierMagnitude = FScalableFloat(10.f);
		// 등록
		Execution.CalculationModifiers.Add(ModifierInfo);
		Executions.Add(Execution);
	}

	// 큐 등록
	FGameplayEffectCue HitCue;
	HitCue.GameplayCueTags.AddTag(GameplayCue::GameplayCue_Debuff_Burn);
	HitCue.MinLevel = 1.f;
	HitCue.MaxLevel = 5.f;
	GameplayCues.Add(HitCue);

	// 5초 지속시간 고정
	DurationMagnitude = FScalableFloat(5.f);
}

UGE_PoisonState::UGE_PoisonState()
{
	// 타겟 부여 태그
	FInheritedTagContainer GrantedTags;
	GrantedTags.AddTag(Debuff::Effect_Debuff_Poison);

	// 타겟에게 부여할 태그 
	auto* TTGEC = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsGameplayEffectComponent"));
	TTGEC->SetAndApplyTargetTagChanges(GrantedTags);
	GEComponents.Add(TTGEC);

	// 상태이상 나이아가라 스폰
	auto* SAAGEC = CreateDefaultSubobject<USpawnNiagaraAttachGEComponent>(TEXT("SpawnActorAttachGEComponent"));
	FSpawnAttachedNiagaraParameters Params;
	Params.mNiagaraAsset = FSoftObjectPath(TEXT("/Script/Niagara.NiagaraSystem'/Game/Asset/Effect/M5VFXVOL2/Niagara/Reference/Fireloop/3_NblowingfireB_fwd_pt.3_NblowingfireB_fwd_pt'"));
	Params.mHasToAttach = true;
	Params.mTransformType = ESpawnTransformType::Target;
	Params.mAttachType = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Params.mSocketName = TEXT("pelvis");
	Params.mAttachType.ScaleRule = EAttachmentRule::KeepWorld;
	SAAGEC->SetParameters(Params);
	GEComponents.Add(SAAGEC);

	// 데미지
	{
		// 계산 클래스 설정
		FGameplayEffectExecutionDefinition Execution;
		Execution.CalculationClass = UGEEC_MaxHPBasedDamage::StaticClass();
		// GEEC 내부 전달 데이터 생성
		FGameplayEffectExecutionScopedModifierInfo ModifierInfo;
		// 수치 처리방식
		ModifierInfo.ModifierOp = EGameplayModOp::AddBase;
		// 값 식별 태그
		ModifierInfo.TransientAggregatorIdentifier = Effect::Effect_Param_DamageRate;
		// 임시 데이터 공간(휘발성)
		ModifierInfo.AggregatorType = EGameplayEffectScopedModifierAggregatorType::Transient;
		// 값 설정
		ModifierInfo.ModifierMagnitude = FScalableFloat(0.01f);
		// 등록
		Execution.CalculationModifiers.Add(ModifierInfo);
		Executions.Add(Execution);
	}

	// 큐 등록
	FGameplayEffectCue HitCue;
	HitCue.GameplayCueTags.AddTag(GameplayCue::GameplayCue_Debuff_Poison);
	HitCue.MinLevel = 1.f;
	HitCue.MaxLevel = 5.f;
	GameplayCues.Add(HitCue);

	// 5초 지속시간 고정
	DurationMagnitude = FScalableFloat(5.f);
}

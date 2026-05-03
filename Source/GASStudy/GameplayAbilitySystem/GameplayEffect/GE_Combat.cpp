// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/GE_Combat.h"
#include "Shared/Tag/SharedTag.h"
#include "GameplayAbilitySystem/GameplayEffect/Components/KnockbackGEComponent.h"

#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

UGE_Combat::UGE_Combat()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	UKnockbackGEComponent* KnockbackGEComp = CreateDefaultSubobject<UKnockbackGEComponent>(TEXT("KnockbackGEComponent"));
	KnockbackGEComp->SetKnockbackPower(Effect::Effect_Param);
	KnockbackGEComp->SetIgnoreKnockbackZHit(true, 150);
	GEComponents.Add(KnockbackGEComp);

	FGameplayEffectCue HitCue;
	HitCue.GameplayCueTags.AddTag(GameplayCue::GameplayCue_Parry);
	HitCue.MinLevel = 1.f;
	HitCue.MaxLevel = 5.f;
	GameplayCues.Add(HitCue);
}

UGE_Stamina::UGE_Stamina()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat SetByCallerFloat;
	SetByCallerFloat.DataTag = Effect::Effect_Param;
	FGameplayEffectModifierMagnitude ModifierMagnitude(SetByCallerFloat);
	ModifierInfo.ModifierMagnitude = ModifierMagnitude;
	Modifiers.Add(ModifierInfo);
}

UGE_Groggy::UGE_Groggy()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UEnemyAttributeSet::GetGroggyAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat SetByCallerFloat;
	SetByCallerFloat.DataTag = Effect::Effect_Param;
	FGameplayEffectModifierMagnitude ModifierMagnitude(SetByCallerFloat);
	ModifierInfo.ModifierMagnitude = ModifierMagnitude;
	Modifiers.Add(ModifierInfo);
}

UGE_Regen::UGE_Regen()
{
	// 정책 
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	// 실행 주기 
	Period.Value = 0.05f; 
}

void UGE_Regen::AddRegenModifier(FGameplayAttribute Attribute, float Amount)
{
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = Attribute;
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// 전투관련 어빌리티 실행시 정지 
	ModifierInfo.TargetTags.IgnoreTags.AddTag(State::State_Combat);

	// 인자로 받은 값으로 증가시
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(Amount));
	Modifiers.Add(ModifierInfo);
}

UGE_StaminaRegen::UGE_StaminaRegen()
{
	AddRegenModifier(UCharacterAttributeSet::GetStaminaAttribute(), 0.5f);
}

UGE_GroggyRegen::UGE_GroggyRegen()
{
	AddRegenModifier(UEnemyAttributeSet::GetGroggyAttribute(), 0.05f);
}

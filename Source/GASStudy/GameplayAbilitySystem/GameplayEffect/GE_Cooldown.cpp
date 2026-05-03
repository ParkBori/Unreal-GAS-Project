// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/GE_Cooldown.h"
#include "Shared/Tag/SharedTag.h"

UGE_Cooldown::UGE_Cooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat SetByCallerFloat;
	SetByCallerFloat.DataTag = Effect::Effect_Param_Time;
	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);

	bSuppressStackingCues = true;

	StackingType = EGameplayEffectStackingType::None;
}

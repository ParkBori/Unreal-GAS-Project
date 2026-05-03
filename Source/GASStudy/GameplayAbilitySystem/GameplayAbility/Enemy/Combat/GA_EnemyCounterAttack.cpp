// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Enemy/Combat/GA_EnemyCounterAttack.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_EnemyCounterAttack::UGA_EnemyCounterAttack()
{
}

void UGA_EnemyCounterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 방어 or 회피 하나를 고르고, 바로 공격을 이어서 할 수 있게만들기 
}

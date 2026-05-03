// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_DashDodgeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_DashDodgeAttack::UGA_DashDodgeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_DashDodgeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* NewTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DashDodgeAttack"), AttackAnim);

	// 몽타주 재생관련 
	NewTask->OnCompleted.AddDynamic(this, &UGA_DashDodgeAttack::OnCompletedCallback);
	NewTask->OnCancelled.AddDynamic(this, &UGA_DashDodgeAttack::OnCancelledCallback);
	NewTask->OnInterrupted.AddDynamic(this, &UGA_DashDodgeAttack::OnInterruptedCallback);

	NewTask->ReadyForActivation();
}

void UGA_DashDodgeAttack::OnCompletedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_DashDodgeAttack::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_DashDodgeAttack::OnCancelledCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

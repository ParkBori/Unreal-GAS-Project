// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_AttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_AttackBase::UGA_AttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* NewTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DashDodgeAttack"), AttackAnim);

	// 몽타주 재생관련 
	NewTask->OnCompleted.AddDynamic(this, &UGA_AttackBase::OnCompletedCallback);
	NewTask->OnCancelled.AddDynamic(this, &UGA_AttackBase::OnCancelledCallback);
	NewTask->OnInterrupted.AddDynamic(this, &UGA_AttackBase::OnInterruptedCallback);

	NewTask->ReadyForActivation();
}

void UGA_AttackBase::OnCompletedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UGA_AttackBase::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UGA_AttackBase::OnCancelledCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

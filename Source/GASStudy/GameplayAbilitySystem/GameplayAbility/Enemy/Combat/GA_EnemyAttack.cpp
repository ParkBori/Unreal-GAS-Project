// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Enemy/Combat/GA_EnemyAttack.h"

#include "AbilitySystemComponent.h"

#include "Shared/Tag/SharedTag.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

UGA_EnemyAttack::UGA_EnemyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_NormalAttack);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_Attacking);

	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParryReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_BlockReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);

}

void UGA_EnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 이 어빌리티는 쿨다운 이펙트가 있어야해서 커밋을 반듯이 해줘야함!
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	if (mPlayMontages.IsEmpty())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	mCurrentPlayMontageIdx = 0;

	auto* AT_WaitGameplayTagAdded = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, Event::Event_Enemy_PlayNextMontage);
	if (AT_WaitGameplayTagAdded)
	{
		AT_WaitGameplayTagAdded->Added.AddDynamic(this, &UGA_EnemyAttack::Callback_AddedPlayNextMontaGeTag);
		AT_WaitGameplayTagAdded->ReadyForActivation();
	}

	mPlayMontage = mPlayMontages[mCurrentPlayMontageIdx];
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_EnemyAttack::Callback_AddedPlayNextMontaGeTag()
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(Event::Event_Enemy_PlayNextMontage);


	++mCurrentPlayMontageIdx;

	// 더 이상 재생할 몽타주가 없음 
	if (!mPlayMontages.IsValidIndex(mCurrentPlayMontageIdx))
	{
		mCurrentPlayMontageIdx = 0;
		return;
	}

	PlayMontage(mPlayMontages[mCurrentPlayMontageIdx]);
}



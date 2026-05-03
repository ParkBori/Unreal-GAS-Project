// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_PlayerResurrection.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"


UGA_PlayerResurrection::UGA_PlayerResurrection()
{
	
}

void UGA_PlayerResurrection::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 플레이어에게 적용시킬 이펙트
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(mResurrectionEffect, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, NewHandle);
	}


	UAbilityTask_PlayMontageAndWait* AT_PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDoDgeAnim"), mPlayerDownToUp);
	AT_PlayMontage->OnCompleted.AddDynamic(this, &UGA_PlayerResurrection::OnMontageCompletedCallback);
	AT_PlayMontage->OnInterrupted.AddDynamic(this, &UGA_PlayerResurrection::OnMontageCompletedCallback);

	AT_PlayMontage->ReadyForActivation();
}

void UGA_PlayerResurrection::OnMontageCompletedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


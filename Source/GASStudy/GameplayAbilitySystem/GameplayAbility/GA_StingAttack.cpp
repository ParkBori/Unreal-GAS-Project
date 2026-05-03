// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_StingAttack.h"
#include "Character/RPGCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


/*
* Sting(찌르기) 는 플레이어와 적의 걸리가 멀어졌을때 작동된다.
* 주요 기능, 시전전에 플레이어를 향해서 회전이 필요하다.
* 찌르기 공격중 괴적이 변경되면 이상하기 때문 
* 
* 애니메이션은 이동부분 + 찌르기 부분으로 나뉜다.
* 이동부분에서는 모션 워핑을 사용하여,
* 기존의 루트모션 이동량보다 많이 이동시킨다. 
* 
*/

UGA_StingAttack::UGA_StingAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_StingAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ARPGCharacter* RPGCharacter = CastChecked<ARPGCharacter>(ActorInfo->AvatarActor.Get());

	// 첫입력 
	UAbilityTask_PlayMontageAndWait* NewTask =  UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), AttackMontage, 1.0f, TEXT("Ready"));
	//NewTask->OnCompleted.AddDynamic(this, &UGA_StingAttack::UGA_StingAttack);
	//NewTask->OnInterrupted.AddDynamic(this, &UGA_StingAttack::UGA_StingAttack);
	NewTask->ReadyForActivation();
}

void UGA_StingAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	//UE_LOG(LogTemp, Display, TEXT("GA_Attack Canceled"));
}

void UGA_StingAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	JumpToSection = false;
	//UE_LOG(LogTemp, Display, TEXT("GA Attack End"));
}

bool UGA_StingAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	//UE_LOG(LogTemp, Display, TEXT("GA_Attack CanAtivate? %s"), (Result ? TEXT("True") : TEXT("False")));
	return Result;
}

void UGA_StingAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UE_LOG(LogTemp, Display, TEXT("Attack InputPressed"));
	
	if (JumpToSection == false)
	{
		JumpToSection = true;
		MontageJumpToSection(TEXT("Attack"));
	}
}

void UGA_StingAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UGA_StingAttack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool WasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, WasCanceled);
}

void UGA_StingAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool WasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, WasCanceled);
}

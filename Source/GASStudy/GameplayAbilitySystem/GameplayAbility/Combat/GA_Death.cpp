// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_Death.h"
#include "Components/CapsuleComponent.h"


UGA_Death::UGA_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Death);
	SetAssetTags(AbilityTag);


	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Event::Event_Ability_Death;
	AbilityTriggers.Add(TriggerData);

	// 발동시 취소시킬 어빌리티
	BlockAbilitiesWithTag.AddTag(Ability::Ability_Combat);
	BlockAbilitiesWithTag.AddTag(Ability::Ability_Jump);

	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Jump);
}

void UGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 죽었을때 처리되어야 하는 것.

	MY_LOG(Error, "");

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_Dodge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Shared/Tag/SharedTag.h"


UGA_Dodge::UGA_Dodge()
{
	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_Dodge);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_Dodging);

	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParryReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_BlockReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_NormalAttack);
}

void UGA_Dodge::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	RePressed();
}

UAnimMontage* UGA_Dodge::SelectMontage_Implementation()
{
	return nullptr;
}

void UGA_Dodge::RePressed_Implementation()
{
}

void UGA_Dodge::SetRotation_Implementation(bool IsPlus)
{
}


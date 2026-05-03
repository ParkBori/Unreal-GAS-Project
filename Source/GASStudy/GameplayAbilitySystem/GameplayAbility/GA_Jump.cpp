// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_Jump.h"
#include "GameFramework/Character.h"
#include "GameplayAbilitySystem/GameplayAbility/GameplayTask/AT_JumpAndWaitForLanding.h"
#include "Shared/Tag/SharedTag.h"

UGA_Jump::UGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 에셋 태그
	SetAssetTags(FGameplayTagContainer(Ability::Ability_Jump));

	// 점프 중 전투 어빌리티 블럭 
	BlockAbilitiesWithTag.AddTag(Ability::Ability_Combat);

	// 점프 중 전투 어빌리티
	SourceBlockedTags.AddTag(Ability::Ability_Combat);
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//UAT_JumpAndWaitForLanding* Task = UAT_JumpAndWaitForLanding::CreateTask(this);
	//Task->OnComplete.AddDynamic(this, &UGA_Jump::OnCompleteCallback);
	//Task->ReadyForActivation();
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	return (Character && Character->CanJump());
}

void UGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UGA_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Jump::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool WasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, WasCanceled);
}

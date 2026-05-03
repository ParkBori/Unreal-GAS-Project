// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_JumpAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_JumpAttack::UGA_JumpAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	StartSection = TEXT("First");
	CurrentSection = StartSection;
}

void UGA_JumpAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* NewTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DashDodgeAttack"), AttackAnim, 1.0f, StartSection);
	
	// 몽타주 섹션 넘겨주는 로직이 필요함 

	// 몽타주 재생관련 
	NewTask->OnCompleted.AddDynamic(this, &UGA_JumpAttack::OnCompletedCallback);
	NewTask->OnCancelled.AddDynamic(this, &UGA_JumpAttack::OnCancelledCallback);
	NewTask->OnInterrupted.AddDynamic(this, &UGA_JumpAttack::OnInterruptedCallback);

	NewTask->ReadyForActivation();

	ACharacter* OnwerCharacter = CastChecked<ACharacter>(CurrentActorInfo->AvatarActor);
	OnwerCharacter->LandedDelegate.AddDynamic(this, &UGA_JumpAttack::OnLanded);
}

void UGA_JumpAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (IGASCanInputInterface::GetCanInput())
	{
		CurrentSection == StartSection ? CurrentSection = TEXT("Second") : CurrentSection = StartSection;

		// 이렇게 작성한 이유는 공중에서 공격하는 모션이 두가지인데, 체공 시간이 길어서 2 번이상의 공격이 가능해지면 
		// 1 -> 2 -> 1 -> 2 의 반복이 가능하게 만들기 위해서임 
		MontageJumpToSection(CurrentSection);
		// 종료후 재시작
		UE_LOG(LogTemp, Display, TEXT("trigger"));
		SetCanInput(false);
	}
}

void UGA_JumpAttack::OnCompletedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_JumpAttack::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_JumpAttack::OnCancelledCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_JumpAttack::OnLanded(const FHitResult& Hit)
{
	ACharacter* OnwerCharacter = CastChecked<ACharacter>(CurrentActorInfo->AvatarActor);
	//MontageJumpToSection(FName("Land"));
	OnwerCharacter->LandedDelegate.RemoveDynamic(this, &UGA_JumpAttack::OnLanded);
}

void UGA_JumpAttack::SetCanInput(bool InCanInput)
{
	IGASCanInputInterface::CanInput = InCanInput;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GameplayTask/AT_PlayMontage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"


UAT_PlayMontage* UAT_PlayMontage::Create(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{

	UAT_PlayMontage* MyObj = NewAbilityTask<UAT_PlayMontage>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}

void UAT_PlayMontage::Activate()
{
	if (!Ability)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (PlayMontage())
	{

		InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAT_PlayMontage::OnGameplayAbilityCancelled);

		SetMontageDelegate();

		bPlayedMontage = true;
	}

	if (!bPlayedMontage)
	{
		// 여기 도달했으면 몽타주 재생에 실패한거이므로 해당 테스크를 종료시킴
		ABILITY_LOG(Warning, TEXT("UAT_PlayMontage called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
		OnCancelled.Broadcast();
	}

	SetWaitingOnAvatar();
}

bool UAT_PlayMontage::PlayMontage()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance)
		{
			ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds);
			return true;
		}
		else
		{
			ABILITY_LOG(Warning, TEXT("UAT_PlayMontage call to PlayMontage failed!"));
			return false;
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UAT_PlayMontage called on invalid AbilitySystemComponent"));
	}

	return false;
}

void UAT_PlayMontage::OnGameplayAbilityCancelled()
{
	EndTask();
}

void UAT_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		// 몽타주 중간 종료 
		//UE_LOG(LogTemp, Display, TEXT("몽타주 인터럽트"));
		OnInterrupted.Broadcast();
	}
	else
	{
		// 몽타주 완전 재생 종료
		//UE_LOG(LogTemp, Display, TEXT("몽타주 종료"));
		OnCompleted.Broadcast();
		EndTask();
	}
}

void UAT_PlayMontage::ChangeMontage(UAnimMontage* NewMontage)
{
	MontageToPlay = NewMontage;

	if (PlayMontage())
	{
		SetMontageDelegate();
	}
}

void UAT_PlayMontage::SetMontageDelegate()
{

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		
		MontageEndedDelegate.BindUObject(this, &UAT_PlayMontage::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_AttackHitCheck.h"


#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "GameplayAbilitySystem/GameplayAbility/GameplayTask/AT_WeaponTrace.h"
#include "GameplayAbilitySystem/GameplayAbility/GameplayTargetActor/TA_WeaponTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Tag/GameplayTag.h"

#include "Shared/Tag/SharedTag.h"

#include "GameplayAbilitySystem/GameplayEffect/GE_Damage.h"

#include "Component/CombatComponent.h"

#include "Shared/GAS/GASData.h"


UGA_AttackHitCheck::UGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Event::Event_StartAttackHitCheck;
	AbilityTriggers.Add(TriggerData);
}

void UGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrnetComboLevel = TriggerEventData->EventMagnitude;

	UE_LOG(LogTemp, Display, TEXT("AttackHitCheck"));

	// 충돌 정보 받기
	UAT_WeaponTrace* AT_WeaponTrace = UAT_WeaponTrace::CreateTask(this, ATA_WeaponTrace::StaticClass(), *TriggerEventData);
	AT_WeaponTrace->mOnTargetDataReady.AddUObject(this, &UGA_AttackHitCheck::Callback_OnTraceResult);
	AT_WeaponTrace->ReadyForActivation();

	// 종료 이벤트 받기
	UAbilityTask_WaitGameplayEvent* AT_WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Event::Event_EndAttackHitCheck);
	AT_WaitGameplayEvent->EventReceived.AddDynamic(this, &UGA_AttackHitCheck::Callback_EndEventReceived);
	AT_WaitGameplayEvent->ReadyForActivation();
}

void UGA_AttackHitCheck::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActorInfo->AvatarActor.IsValid())
	{
		mCombatComp = ActorInfo->AvatarActor->FindComponentByClass<UCombatComponent>();
	}
}

void UGA_AttackHitCheck::Callback_OnTraceResult(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	int DataSize = TargetDataHandle.Data.Num();

	for (int i = 0; i < DataSize; ++i)
	{
		if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, i))
		{
			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

			AActor* SourceActor = CurrentActorInfo->AvatarActor.Get();
			AActor* TargetActor = HitResult.GetActor();

			// UE_LOG(LogTemp, Display, TEXT("GA_AttackHitCheck HitActor %s"), *TargetActor->GetName());
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!SourceASC || !TargetASC)
			{
				return;
			}

			// 리액션 처리
			if (UCombatComponent* Combat = TargetActor->FindComponentByClass<UCombatComponent>())
			{
				FGameplayEventData EventData;
				EventData.Instigator = SourceActor;
				EventData.Target = TargetActor;

				FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
				Context.AddInstigator(SourceActor, SourceActor);
				Context.AddSourceObject(SourceActor);
				Context.AddHitResult(HitResult);
				EventData.ContextHandle = Context;

				// 현재 공격 어빌리티 정보를 받아와서, 그것을 기반으로 
			

				Combat->HandleIncomingAttack(SourceActor, EventData);
			}
		}
	}
}

void UGA_AttackHitCheck::Callback_EndEventReceived(FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_TestAbilityTemplate.h"
#include "Tag/GameplayTag.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


//#include "Abilities/Tasks/"

UGA_TestAbilityTemplate::UGA_TestAbilityTemplate()
{
	// 어빌리티 실행될때마다 인스턴스 생성 
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	
	// 태그 설정 

	// 기본태그
	FGameplayTagContainer DefaultsTags;
	//DefaultsTags.AddTag(TAG_TESTS_COMBAT_BLOCK);
	SetAssetTags(DefaultsTags);
	
	// 활성화 소유 태그 
	//ActivationOwnedTags.AddTag(TAG_TESTS_COMBAT_BLOCK);

	// 트리거 설정
	FAbilityTriggerData TriggerData;
	//TriggerData.TriggerTag = TAG_TESTS_COMBAT_BLOCK;
	AbilityTriggers.Add(TriggerData);

	static ConstructorHelpers::FClassFinder<UGameplayEffect> CostEffectAsset(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/AbilitySystem/MeleeAttack/GE_AttackStamina.GE_AttackStamina_C'"));
	if (CostEffectAsset.Succeeded())
	{
		CostGameplayEffectClass = CostEffectAsset.Class;
	}

	// 쿨다운 이펙트 추가
	//static ConstructorHelpers::FClassFinder<UGameplayEffect> CooldownEffectAsset(TEXT(""));
	//if (CooldownEffectAsset.Succeeded())
	//{
	//	CooldownGameplayEffectClass = CooldownEffectAsset.Class;
	//}
}

bool UGA_TestAbilityTemplate::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - CanActivateAbility"));
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_TestAbilityTemplate::CallActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - CallActivateAbility"));
	Super::CallActivateAbility(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UGA_TestAbilityTemplate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - ActivateAbility"));
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Cost 나 CoolDown 이 존재한다면,  Commity Ability 직접 호출 필요 
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}

	// 데미지 적용 
	/*UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(HitGameplayEffect, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, NewHandle);
	}*/



	WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_EVENT_TESTEVENT, nullptr, true, true);
	WaitEvent->EventReceived.AddDynamic(this, &UGA_TestAbilityTemplate::WaitEventCallBackFunc);
	WaitEvent->ReadyForActivation();

	//ndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGA_TestAbilityTemplate::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - CommitAbility"));
	return	Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

bool UGA_TestAbilityTemplate::CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool ForceCooldown, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - CommitAbilityCooldown"));
	return Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, ForceCooldown, OptionalRelevantTags);
}

bool UGA_TestAbilityTemplate::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - CommitAbilityCost"));
	return Super::CommitAbilityCost(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

void UGA_TestAbilityTemplate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - CancelAbility"));
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_TestAbilityTemplate::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - EndAbility"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 자동적으로 호출해준다?
	if (WaitEvent)
	{
		WaitEvent->EndTask();
	}

	if (bWasCancelled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Ability"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Success Ability"));
	}
}

void UGA_TestAbilityTemplate::WaitEventCallBackFunc(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_EnemyBlock - Tag Catch"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

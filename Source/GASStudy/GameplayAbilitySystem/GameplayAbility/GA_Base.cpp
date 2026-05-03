// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "GameplayAbilitySystem/GASStudyAbilitySystemGlobals.h"

void UGA_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Cost 나 CoolDown 이 존재한다면,  Commity Ability 직접 호출 필요 
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UGA_Base::ApplySetByCallerEffect(TSubclassOf<UGameplayEffect> EffectClass, const TArray<FSetByCallerData>& SetByCallerData)
{
	if (!EffectClass)
	{
		return;
	}

	if(SetByCallerData.IsEmpty())
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		EffectClass,
		1,
		EffectContext
	);
	
	if (!SpecHandle.IsValid())
	{
		return;
	}

	for (const auto& Data : SetByCallerData)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(Data.DataTag, Data.Value);
	}
	
	ApplyGameplayEffectSpecToOwner(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		SpecHandle
	);
}

UGA_PlayMontage::UGA_PlayMontage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 일반적으로 싱글플레이 게임의 경우는 LoacalOnly 를 사용
	// NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
	// 싱글플레이에서는 로컬 머신이 서버 역할을 하기 때문에, 안전성을 위해 이 정책을 선택할 수 있음.
	// 특히, 복잡한 계산이나 치트 방지가 필요한 로직(비록 싱글플레이일지라도)을 서버 측 로직으로 처리하고 싶을 때 고려.
	// NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	// 어빌리티 태그는 서브클래스에서 정의
}

void UGA_PlayMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	mMontageLevel = 0;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!mPlayMontage)
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	// 현재 실행 중인 모든 어빌리티 스펙을 순회
	//TArray<FGameplayAbilitySpec> ActiveSpecs = ASC->GetActivatableAbilities();
	//for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	//{
	//	// 1. 나 자신은 제외
	//	// 2. 실행 중이며(IsActive), 몽타주를 사용하는 어빌리티(UGA_PlayMontage)인지 확인
	//	if (Spec.IsActive() && Spec.Ability != this && Spec.Ability->IsA<UGA_PlayMontage>())
	//	{
	//		// 강제로 취소 시킴 -> 이때 해당 어빌리티의 EndAbility가 호출됩니다.
	//		ASC->CancelAbilityHandle(Spec.Handle);
	//	}
	//}

	PlayMontage(mPlayMontage);
}

void UGA_PlayMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_PlayMontage::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (!MontageToPlay) return;

	mPlayMontage = MontageToPlay;

	mMontageLevel++;

	// MY_LOG(Error, "MontageLevel : %d", mMontageLevel);

	if (mAT_PlayMontageTask)
	{
		mAT_PlayMontageTask->OnCompleted.Clear();
		mAT_PlayMontageTask->OnInterrupted.Clear();
		mAT_PlayMontageTask->OnCancelled.Clear();
		mAT_PlayMontageTask->EndTask();
		mAT_PlayMontageTask = nullptr;
	}

	mAT_PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), MontageToPlay, PlayRate, mSectionName);
	if (IsValid(mAT_PlayMontageTask))
	{
		mAT_PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::Callback_OnCompletedMontage);
		mAT_PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::Callback_OnInterruptedMontage);
		mAT_PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::Callback_OnCancelledMontage);
		mAT_PlayMontageTask->ReadyForActivation();
	}

}

void UGA_PlayMontage::Callback_OnInterruptedMontage()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_PlayMontage::Callback_OnCancelledMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_PlayMontage::Callback_OnCompletedMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

UGA_Combat::UGA_Combat()
{
	// 활성 태그 
	ActivationOwnedTags.AddTag(State::State_Combat);

	// 취소태그
	CancelAbilitiesWithTag.AddTag(State::State_Cancelable);

	mAlwaysExcute.AddTag(Ability::Ability_Combat_Hit);
	mAlwaysExcute.AddTag(Ability::Ability_Combat_ParryReaction);
	mAlwaysExcute.AddTag(Ability::Ability_Combat_BlockReaction);
	mAlwaysExcute.AddTag(Ability::Ability_Combat_ParriedReaction);
	mAlwaysExcute.AddTag(Ability::Ability_Combat_Finished);
	mAlwaysExcute.AddTag(Ability::Ability_Combat_Finisher);

	ActivationBlockedTags.AddTag(State::State_Dead);

	// 전투 중 점프 어빌리티 블럭 
	BlockAbilitiesWithTag.AddTag(Ability::Ability_Jump);

	// 전투 어빌리티 중 점프 막기
	SourceBlockedTags.AddTag(Ability::Ability_Jump);
}

bool UGA_Combat::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	bool bSuperResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bSuperResult)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const FGameplayTagContainer& TagContainer = ASC->GetOwnedGameplayTags();

	if (TagContainer.HasTag(State::State_Dead))
	{
		return false;
	}

	if (TagContainer.HasTagExact(State::State_Combat))
	{
		// mAlwaysExcute 태그를 가진 어빌리티(Hit 등)는 언제나 허용
		if (mAlwaysExcute.HasAnyExact(GetAssetTags()/*AbilityTags*/))
		{
			if (mAlwaysExcute.HasTagExact(Ability::Ability_Combat_Groggy))
			{
				int a = 3;
			}
			return true;
		}
		else
		{
			// 캔슬 윈도우가 아닐 때는 실행 차단
			if (!TagContainer.HasTagExact(State::State_CancelWindow))
			{
				return false;
			}
		}
	}

	return bSuperResult;
}


void UGA_Combat::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActorInfo->AvatarActor.IsValid() && bIsAttackAbility && !mAttackName.IsNone())
	{
		UDataTable* AttackData = UGASStudyAbilitySystemGlobals::Get().GetGlobalSkillDataTable();
		if (AttackData)
		{
			mCombatComp = ActorInfo->AvatarActor->FindComponentByClass<UCombatComponent>();
			mAttackDataRow = AttackData->FindRow<FGASAttackAbilityData>(mAttackName, TEXT("Isn't Row Data"));
		}
	}
}

void UGA_Combat::PlayMontage(UAnimMontage* MontageToPlay)
{
	Super::PlayMontage(MontageToPlay);
	if (mCombatComp.IsValid())
	{
		mCombatComp->SetMontageLevel(mMontageLevel);
	}
}

void UGA_Combat::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (mCombatComp.IsValid() && mAttackDataRow)
	{
		mCombatComp->SetAttackDataRow(mAttackDataRow);
	}

	UAbilityTask_WaitGameplayTagAdded* AT_WaitTagAdded = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, State::State_CancelWindow);
	if (AT_WaitTagAdded)
	{
		AT_WaitTagAdded->Added.AddDynamic(this, &UGA_Combat::Callback_AddedCancelWindowTag);
		AT_WaitTagAdded->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayTagRemoved* AT_WaitTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, State::State_CancelWindow);
	if (AT_WaitTagRemoved)
	{
		AT_WaitTagRemoved->Removed.AddDynamic(this, &UGA_Combat::Callback_RemovedCancelWindowTag);
		AT_WaitTagRemoved->ReadyForActivation();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Combat::Callback_AddedCancelWindowTag()
{
	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (!Spec)
	{
		return;
	}

	Spec->GetDynamicSpecSourceTags().AddTag(State::State_Cancelable);
}

void UGA_Combat::Callback_RemovedCancelWindowTag()
{
	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (!Spec)
	{
		return;
	}

	Spec->GetDynamicSpecSourceTags().RemoveTag(State::State_Cancelable);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_Block.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTagCountChanged.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "GameFramework/Character.h"

#include "Shared/Macro/Log.h"

UGA_Block::UGA_Block()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_Block);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	// 실제 플레이의 키입력에 따라 부여되는 태그이기 때문에 직접 관리해야한다
    // ActivationOwnedTags.AddTag(State::State_Blocking);

    CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
    CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_NormalAttack);

    CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
}

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // 최초 입력 → Parry Window 시작

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }
    
    mBlockState = EBlockState::Start;

    ASC->AddLooseGameplayTag(State::State_Blocking);
    
    StartParryWindow();
    
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Block::InputPressed(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        return;
    }

    // 재입력 시점에 종료 이벤트가 등록되어있다면 제거해줘야한다.
    if (IsValid(mAT_WaitTagRemoved))
    {
        mAT_WaitTagRemoved->EndTask();
        mAT_WaitTagRemoved = nullptr;
    }


    mBlockState = EBlockState::Loop;

    ASC->RemoveLooseGameplayTag(State::State_Blocking);
    ASC->AddLooseGameplayTag(State::State_Blocking);


    // 다시 패리가능 상태를 부여해준다.
    StartParryWindow();


    // 패리어빌리티가 재생되지 않을때만 실행
    if (!ASC->HasMatchingGameplayTag(State::State_ParryReactiing))
    {
        mBlockState = EBlockState::Start;
        // 블럭 모션을 취하는 애니메이션 다시 실행
        PlayMontage(mPlayMontage);
    }
}

void UGA_Block::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    // 어빌리티 종료 트리거

    MY_LOG(Error, "");

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        return;
    }

    mBlockState = EBlockState::End;

    // 패링, 블럭 태그
    ASC->RemoveLooseGameplayTag(State::State_Blocking);
    ASC->RemoveLooseGameplayTag(State::State_ParryWindow);


    if (ASC->HasMatchingGameplayTag(State::State_ParryReactiing))
    {
        // 패리 리액션 어빌리티가 종료될때까지 기다린 이후에 종료실행
        mAT_WaitTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, State::State_ParryReactiing);
        if (mAT_WaitTagRemoved)
        {
            MY_LOG(Error, "패링 종료 대기 후 종료");
            mAT_WaitTagRemoved->Removed.AddDynamic(this, &UGA_Block::Callback_TagRemoved);
            mAT_WaitTagRemoved->ReadyForActivation();
        }
    }

    else
    {
        MY_LOG(Error, "즉시 종료");
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
}

void UGA_Block::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (ASC)
    {
        ASC->RemoveLooseGameplayTag(State::State_Blocking);
        ASC->RemoveLooseGameplayTag(State::State_ParryWindow);
    }

    GetWorld()->GetTimerManager().ClearTimer(mParryTimerHandle);

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Block::StartParryWindow()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        return;
    }

    // 기존 존재하는 태그가 있을 경우를 대비 미리 제거
    ASC->RemoveLooseGameplayTag(State::State_ParryWindow);

    // 패리 태그 부여
    ASC->AddLooseGameplayTag(State::State_ParryWindow);

    // 쿨타임 설정 (연타 방지)
    //bParryOnCooldown = true;

    // 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        mParryTimerHandle,
        this,
        &UGA_Block::EndParryWindow,
        mParryWindow,
        false
    );
}

void UGA_Block::EndParryWindow()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        return;
    }

    // 패리 태그 제거 
    ASC->RemoveLooseGameplayTag(State::State_ParryWindow);
}

void UGA_Block::Callback_OnCompletedMontage()
{
    if (mBlockState == EBlockState::End)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }

    mBlockState = EBlockState::Loop;
}

void UGA_Block::Callback_OnInterruptedMontage()
{
    if (mBlockState == EBlockState::End)
    {
        // End가 끊겼으면 그냥 종료
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }

    mBlockState = EBlockState::Loop;
}

void UGA_Block::Callback_OnCancelledMontage()
{
    Callback_OnInterruptedMontage();
}

void UGA_Block::Callback_TagRemoved()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

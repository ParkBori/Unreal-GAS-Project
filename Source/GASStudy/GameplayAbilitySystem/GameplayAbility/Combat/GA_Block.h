// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_Block.generated.h"


enum class EBlockState : uint8
{
    Start,
    Loop,
    End,
};


/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_Block : public UGA_Combat
{
	GENERATED_BODY()
	
public:
    UGA_Block();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

    virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
    void StartParryWindow();
    void EndParryWindow();

private:
    virtual void Callback_OnCompletedMontage() override;
    virtual void Callback_OnInterruptedMontage() override;
    virtual void Callback_OnCancelledMontage() override;

    UFUNCTION()
    void Callback_TagRemoved();

private:
    EBlockState mBlockState = EBlockState::Start;

    FTimerHandle mParryTimerHandle;

    float mParryWindow = 0.4f;


    UPROPERTY()
    TObjectPtr<class UAbilityTask_WaitGameplayTagRemoved> mAT_WaitTagRemoved;

};

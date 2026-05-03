// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Enemy/Combat/GA_Groggy.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

UGA_Groggy::UGA_Groggy()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    SetAssetTags(FGameplayTagContainer(Ability::Ability_Combat_Groggy));

    PlayRate = 0.1f;
}

void UGA_Groggy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Groggy::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (ASC)
    {
        // AttributeSet 가져오기
        const UEnemyAttributeSet* AS = ASC->GetSet<UEnemyAttributeSet>();
        if (AS)
        {
            // 그로기 종료 시 최대값으로 복구
            float MaxGroggy = AS->GetMaxGroggy();
            ASC->SetNumericAttributeBase(AS->GetGroggyAttribute(), AS->GetMaxGroggy());
        }
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



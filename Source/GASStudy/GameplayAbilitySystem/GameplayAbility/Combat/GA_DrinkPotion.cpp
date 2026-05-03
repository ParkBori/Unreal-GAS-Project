// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_DrinkPotion.h"

#include "Shared/Macro/Log.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Playable/PlayableCharacter.h"
#include "Component/InventoryComponent_V2.h"


UGA_DrinkPotion::UGA_DrinkPotion()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // 기본태그
    FGameplayTagContainer AbilityTag;
    AbilityTag.AddTag(Ability::Ability_Combat_DrinkPotion);
    AbilityTag.AddTag(Ability::Ability_Combat);
    SetAssetTags(AbilityTag);

    // 활성화 소유 태그 
    ActivationOwnedTags.AddTag(State::State_DrinkingPotion);

    // 이벤트 트리거 태그
}

void UGA_DrinkPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    auto Inventory = GetInventory();
    if (!IsValid(Inventory))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }

    if (!Inventory->CanUseQuickSlotItem(0, 1))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }

    auto* AT_WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Event::Event_Ability_ItemActivationTime, nullptr, true);
    if (AT_WaitEvent)
    {
        AT_WaitEvent->EventReceived.AddDynamic(this, &UGA_DrinkPotion::Callback_EventReceived);
        AT_WaitEvent->ReadyForActivation();
    }

    // 손에 아이이템 부착

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_DrinkPotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // 손 아이템 부착 해제
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UInventoryComponent_V2* UGA_DrinkPotion::GetInventory()
{
    AActor* Source = GetAvatarActorFromActorInfo();
    if (!Source)
    {
        return nullptr;
    }

    // 인벤토리 얻기 
    auto* Player = Cast<APlayableCharacter>(Source);
    if (!Player)
    {
        return nullptr;
    }

    return Player->GetInventoryComponent();
}

void UGA_DrinkPotion::Callback_EventReceived(FGameplayEventData Payload)
{
    auto Inventory = GetInventory();
    if (!IsValid(Inventory))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }
    // 아이템 개수 줄이기 
    bool bSuccess = Inventory->UseItem(0, 1);
    if (!bSuccess)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }
}


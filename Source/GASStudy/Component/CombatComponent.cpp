// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CombatComponent.h"
#include "Shared/Tag/SharedTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/GameplayEffect/GE_Damage.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;

	// ...
}

void UCombatComponent::InitializeComponent()
{
    Super::InitializeComponent();

    if (AActor* Owner = GetOwner())
    {
        mASC = Owner->FindComponentByClass<UAbilitySystemComponent>();
    }
}

EAttackResult UCombatComponent::HandleIncomingAttack(AActor* Attacker, const FGameplayEventData& EventData)
{
    EAttackResult Result = ResolveAttackResult(Attacker);
    

    if (GetOwner()->ActorHasTag(TEXT("Player")))
    {
        // FText로 가져오기 (다국어 지원 및 UI 출력용)
        FText ResultText = UEnum::GetDisplayValueAsText(Result);
        // 로그 출력을 위해 FString으로 변환
        UE_LOG(LogTemp, Warning, TEXT("공격 결과: %s"), *ResultText.ToString());
    }

    switch (Result)
    {
    case EAttackResult::Parry:
        HandleParry(Attacker, EventData);
        break;

    case EAttackResult::Block:
        HandleBlock(Attacker, EventData);
        break;

    default:
        HandleHit(Attacker, EventData);
        break;
    }

    return Result;
}

void UCombatComponent::SetCurrentBlockPosition(EBlockPosition NewPosition) 
{
    mCurrentBlockPosition = NewPosition;
}

EBlockPosition UCombatComponent::GetCurrentBlockPosition() const
{
    return mCurrentBlockPosition;
}


EAttackResult UCombatComponent::ResolveAttackResult(AActor* Attacker)
{
    if (!mASC.IsValid())
        return EAttackResult::Hit;

    const FGameplayTagContainer& Tags = mASC->GetOwnedGameplayTags();

    // 공격은 어빌리티를 통해서만 전달.
    auto* AttackerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Attacker);
    FGameplayTagContainer AttackerTags;
    if (AttackerASC)
    {
        AttackerTags = AttackerASC->GetOwnedGameplayTags();
    }

    if (AttackerTags.HasTagExact(State::State_Critical))
    {
        return EAttackResult::Hit;
    }

    if (Tags.HasTagExact(State::State_ParryWindow))
    {
        return EAttackResult::Parry;
    }

    if (Tags.HasTagExact(State::State_Blocking))
    {
        return EAttackResult::Block;
    }

    return EAttackResult::Hit;
}

void UCombatComponent::HandleParry(AActor* Attacker, const FGameplayEventData& EventData)
{
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), Event::Event_Ability_Combat_ParryReaction, EventData);
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Attacker, Event::Event_Ability_Combat_ParriedReaction, EventData);
}

void UCombatComponent::HandleBlock(AActor* Attacker, const FGameplayEventData& EventData)
{
    // 공격 받은 캐릭터만 작동 
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), Event::Event_Ability_Combat_BlockReaction, EventData);
}

void UCombatComponent::HandleHit(AActor* Attacker, const FGameplayEventData& EventData)
{
    if (mASC->GetOwnedGameplayTags().HasTagExact(State::State_Dead))
    {
        return;
    }
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), Event::Event_Ability_Combat_Hit, EventData);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_BlockReaction.h"
#include "AbilitySystemComponent.h"
#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "Component/CombatComponent.h"
#include "GameplayAbilitySystem/GameplayEffect/GE_Combat.h"


UGA_BlockReaction::UGA_BlockReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_BlockReaction);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_BlockReacting);

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Event::Event_Ability_Combat_BlockReaction;
	AbilityTriggers.Add(TriggerData);

	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParryReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_NormalAttack);

	bRetriggerInstancedAbility = true;
}

void UGA_BlockReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		UGE_Combat::StaticClass(), 1.f, TriggerEventData->ContextHandle);

	SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param, 1000);
	//ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);

	AActor* SourceActor = GetAvatarActorFromActorInfo();
	AActor* TargetActor = TriggerEventData->ContextHandle.GetInstigator();

	mPlayMontage = SelectMontage(SourceActor, *TriggerEventData->ContextHandle.GetHitResult());

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UAnimMontage* UGA_BlockReaction::SelectMontage(const AActor* SourceActor, const FHitResult& HitResult)
{
	EHitDirectionArea HitArea = UMyBlueprintFunctionLibrary::CalculateDirectionToHitResult(SourceActor, HitResult);

	UCombatComponent* CC = SourceActor->FindComponentByClass<UCombatComponent>();
	check(CC);

	switch (HitArea)
	{
	case EHitDirectionArea::FrontRight:
	case EHitDirectionArea::BackRight:
		CC->SetCurrentBlockPosition(EBlockPosition::Right);
		break;
	case EHitDirectionArea::FrontLeft:
	case EHitDirectionArea::BackLeft:
		CC->SetCurrentBlockPosition(EBlockPosition::Left);
		break;
	}

	FText DirText = UEnum::GetDisplayValueAsText(HitArea);
	UE_LOG(LogTemp, Error, TEXT("Attacked Dir : %s"), *DirText.ToString());

	if (!mBlockReactionMontagesByHitArea.Contains(HitArea))
	{
		return nullptr;
	}

	return mBlockReactionMontagesByHitArea[HitArea];
}


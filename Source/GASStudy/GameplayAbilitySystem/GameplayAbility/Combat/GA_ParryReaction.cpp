// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_ParryReaction.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/GameplayEffect/GE_Combat.h"
#include "Component/CombatComponent.h"

UGA_ParryReaction::UGA_ParryReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_ParryReaction);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_ParryReactiing);

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Event::Event_Ability_Combat_ParryReaction;
	AbilityTriggers.Add(TriggerData);

	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_BlockReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_NormalAttack);

	bRetriggerInstancedAbility = true;
}

void UGA_ParryReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Error, TEXT("ParryReaction"));

	// TODO : 
	// 패리성공 게임플레이 큐 재생 
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		UGE_Combat::StaticClass(), 1.f, TriggerEventData->ContextHandle);

	SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param, 1000);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);

	AActor* SourceActor = GetAvatarActorFromActorInfo();
	AActor* TargetActor = TriggerEventData->ContextHandle.GetInstigator();
	
	mPlayMontage = SelectMontage(SourceActor, *TriggerEventData->ContextHandle.GetHitResult());
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UAnimMontage* UGA_ParryReaction::SelectMontage(const AActor* SourceActor, const AActor* TargetActor)
{
	EHitDirection Dir = UMyBlueprintFunctionLibrary::CalculateDirection(SourceActor, TargetActor);
	FText DirText = UEnum::GetDisplayValueAsText(Dir);
	UE_LOG(LogTemp, Error, TEXT("Attacked Dir : %s"), *DirText.ToString());

	if (!mParryReactionMontages.Contains(Dir))
	{
		return nullptr;
	}

	return mParryReactionMontages[Dir];
}

UAnimMontage* UGA_ParryReaction::SelectMontage(const AActor* SourceActor, const FVector TargetLoaction)
{
	EHitDirectionArea HitArea = UMyBlueprintFunctionLibrary::CalculateDirectionToLocation(SourceActor, TargetLoaction);
	FText DirText = UEnum::GetDisplayValueAsText(HitArea);
	UE_LOG(LogTemp, Error, TEXT("Attacked Dir : %s"), *DirText.ToString());

	if (!mParryReactionMontagesByHitArea.Contains(HitArea))
	{
		return nullptr;
	}

	return mParryReactionMontagesByHitArea[HitArea];
}

UAnimMontage* UGA_ParryReaction::SelectMontage(const AActor* SourceActor, const FHitResult& HitResult)
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

	if (!mParryReactionMontagesByHitArea.Contains(HitArea))
	{
		return nullptr;
	}

	return mParryReactionMontagesByHitArea[HitArea];
}


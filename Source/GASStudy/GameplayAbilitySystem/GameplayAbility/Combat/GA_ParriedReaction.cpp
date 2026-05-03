// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_ParriedReaction.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "Component/CombatComponent.h"

#include "GameplayAbilitySystem/GameplayEffect/GE_Combat.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

UGA_ParriedReaction::UGA_ParriedReaction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_ParryReactiing);

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Event::Event_Ability_Combat_ParriedReaction;
	AbilityTriggers.Add(TriggerData);

	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_BlockReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParryReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);
	//CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_NormalAttack);

	bRetriggerInstancedAbility = true;
}

void UGA_ParriedReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Display, TEXT(""));

	AActor* SourceActor = GetAvatarActorFromActorInfo();
	AActor* TargetActor = TriggerEventData->ContextHandle.GetInstigator();
	
	TArray<FSetByCallerData> SetByCallerData;
	SetByCallerData.Add({ Effect::Effect_Param, -30 });
	ApplySetByCallerEffect(UGE_Groggy::StaticClass(), SetByCallerData);

	auto* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC->GetOwnedGameplayTags().HasTagExact(State::State_SuperArmor))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	

	auto EnemyAttributeset = ASC->GetSet<UEnemyAttributeSet>();
	if (IsValid(EnemyAttributeset))
	{
		float CurrentGroggy = EnemyAttributeset->GetGroggy();
		if (CurrentGroggy <= 0)
		{
			// 애니메이션 재생속도 느리게 만들기 
			PlayRate = 0.3f;
			ASC->AddLooseGameplayTag(State::State_Groggy);
		}
	}

	mPlayMontage = SelectMontage(SourceActor, *TriggerEventData->ContextHandle.GetHitResult());
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_ParriedReaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	auto ASC = GetAbilitySystemComponentFromActorInfo();
	if (IsValid(ASC))
	{

		if (ASC->GetTagCount(State::State_Groggy) >= 1)
		{
			// AttributeSet 가져오기
			const UEnemyAttributeSet* AS = ASC->GetSet<UEnemyAttributeSet>();
			if (AS)
			{
				ASC->RemoveLooseGameplayTag(State::State_Groggy);
				// 그로기 종료 시 최대값으로 복구
				float MaxGroggy = AS->GetMaxGroggy();
				ASC->SetNumericAttributeBase(AS->GetGroggyAttribute(), AS->GetMaxGroggy());
			}
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}


UAnimMontage* UGA_ParriedReaction::SelectMontage(const AActor* SourceActor, const FHitResult& HitResult)
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

	if (!mParriedReactionMontagesByHitArea.Contains(HitArea))
	{
		return nullptr;
	}

	return mParriedReactionMontagesByHitArea[HitArea];
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_Hit.h"
#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "GameplayAbilitySystem/GameplayEffect/GE_Damage.h"



/**
* 어떠한 어빌리티가 실행될지는, 외부 mCombatComp 에서 분기처리 돼서, 여기서는 최종적인 실행이 일어나야한다.
* 
* 데미지를 입는 상태가 확정되어 진입했기때문에, 전투와 관련된 어빌리티는 모두 종료 시켜야한다.
*/

UGA_Hit::UGA_Hit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_Hit);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_Hitting);

	ActivationBlockedTags.AddTag(State::State_Dead);

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Event::Event_Ability_Combat_Hit;
	AbilityTriggers.Add(TriggerData);

	// 발동시 취소시킬 어빌리티
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_BlockReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParryReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Block);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_NormalAttack);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Dodge);

	// 작동중인 어빌리티 취소 / 해당 어빌리티 실행중 차단할 어빌리티는 블루프린트에서 설정
	// 재발동
	bRetriggerInstancedAbility = true;
}

void UGA_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AActor* Instigator = TriggerEventData->Instigator;
	if (nullptr == Instigator)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	// 공격자 정보 
	auto* InstigatorCC = Instigator->FindComponentByClass<UCombatComponent>();
	const FGASAttackAbilityData* AttackDataRow = InstigatorCC->GetAttackDataRow();

	float DmgCoefficient = 1.f;
	float KnocbackAmount = 0.f;
	FGASSecondaryEffectData DebuffData;

	if (InstigatorCC)
	{
		if (AttackDataRow && AttackDataRow->AttackDatas.IsValidIndex(0))
		{
			int32 MontageLevel = InstigatorCC->GetMontageLevel();

			DmgCoefficient = AttackDataRow->AttackDatas[0].DamageData.GetValueAtLevel(MontageLevel);
			KnocbackAmount =  AttackDataRow->AttackDatas[0].Knockback.GetValueAtLevel(MontageLevel);
			DebuffData = AttackDataRow->AttackDatas[0].Debuff;
		}
	}

	// 데미지 이펙트 적용
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		UGE_NormalDamage::StaticClass(), 1.f, TriggerEventData->ContextHandle);
	
	SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param_DmgCoefficient, DmgCoefficient);
	SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param_Knockback, KnocbackAmount);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	
	// 디버프 적용
	if (DebuffData.SecondaryEffectClass)
	{
		FGameplayEffectSpecHandle DebuffSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			DebuffData.SecondaryEffectClass, 1.f, TriggerEventData->ContextHandle);

		SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param_DamageRate, DebuffData.Magnitude);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, DebuffSpecHandle);
	}

	auto* ASC = GetAbilitySystemComponentFromActorInfo();
	const FGameplayTagContainer& Tags = ASC->GetOwnedGameplayTags();
	if (Tags.HasTagExact(State::State_SuperArmor)
		|| Tags.HasTagExact(State::State_Groggy)
		|| Tags.HasTagExact(State::State_Dead))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 몽타주 재생 

	mPlayMontage = SelectHitMontage(GetAvatarActorFromActorInfo(), Instigator);
	if (nullptr == mPlayMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}



	//// 부모클래스에서 어빌리티 처리(애니메이션 재생)
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	MY_LOG(Error, "Hit 어빌리티 종료");
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* UGA_Hit::SelectHitMontage(const AActor* Vicitim, const AActor* Instigator)
{
	const auto& ASC = GetAbilitySystemComponentFromActorInfo();
	/*if (ASC->HasMatchingGameplayTag(State::State_MaxPosture))
	{
		float Percent = FMath::RandRange(0.f, 1.0f);
		if (Percent < 0.5f)
		{
			return HitOnMaxPostureState_V1;
		}
		else
		{
			return HitOnMaxPostureState_V2;
		}
	}*/

	EHitDirection HitDir = UMyBlueprintFunctionLibrary::CalculateDirection(Vicitim, Instigator);
	switch (HitDir)
	{
	case EHitDirection::Left:
		return mLeftHit;
	case EHitDirection::Right:
		return mRightHit;
	case EHitDirection::Fwd:
		return mForwardHit;
	case EHitDirection::Back:
		return mBackHit;
	}

	return nullptr;	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_DynamicAttack.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// test // 
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
// test // 



#include "Component/EquipComponent.h"

#include "Shared/Tag/SharedTag.h"

#include "Shared/Macro/Log.h"

#include "Shared/Commbat/ComboAttackData.h"

UGA_DynamicAttack::UGA_DynamicAttack()
{
	// 사용시 기본 태그 / 이벤트 태그 변경 필요 
	// 현재 하나로 되어있음

	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_NormalAttack);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);

	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_Attacking);

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Ability::Ability_Combat_NormalAttack;
	AbilityTriggers.Add(TriggerData);

	// 무기 활성화 상태에서만 작동
	ActivationRequiredTags.AddTag(State::State_WeaponActivated);


	// 테스트용
	//static ConstructorHelpers::FObjectFinder<UDataTable> AnimDTAsset(TEXT("/Script/Engine.DataTable'/Game/GASStudy/Data/Combat/DT_WeaponCombatData.DT_WeaponCombatData'"));
	//if (AnimDTAsset.Succeeded())
	//{
	//	mAnimDT = AnimDTAsset.Object;
	//}
}

bool UGA_DynamicAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_DynamicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	MY_LOG(Error, "EventTag %s  ------ InstiagtorTags %s", *TriggerEventData->EventTag.ToString(), *TriggerEventData->InstigatorTags.ToString());
	bFirst = true;

	if (!mEquipComponent.IsValid())
	{
		mEquipComponent = ActorInfo->AvatarActor->GetComponentByClass<UEquipComponent>();
	}

	if (!mEquipComponent.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!SetComboData(mEquipComponent->GetEquipWeaponTag().GetTagName()))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	mPlayNode = TEXT("Root");

	// 어빌리티가 실행될 때 한 번 호출됨 --> 루트 필요할거같은데?

	// 플레이 몽타주 선택
	SetPlayMontage(TriggerEventData->InstigatorTags);

	// 별도의 태그 지정 필요
	UAbilityTask_WaitGameplayEvent* AT_WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Ability::Ability_Combat_NormalAttack, nullptr, false, true);

	if (!AT_WaitGameplayEvent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AT_WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::Callback_EventReceived);
	AT_WaitGameplayEvent->ReadyForActivation();
}

void UGA_DynamicAttack::Callback_EventReceived(FGameplayEventData Payload)
{
	if (bFirst)
	{
		bFirst = false;
		return;
	}

	SetPlayMontage(Payload.InstigatorTags);

	// 몽타주 생성 
	PlayMontage(mPlayMontage);
}

bool UGA_DynamicAttack::SetComboData(const FName& Key)
{
	if (!mAnimDT)
	{
		return false;
	}
	
	const FDynamicCombatDataByWeapon* Data = mAnimDT->FindRow<FDynamicCombatDataByWeapon>(Key, TEXT(""));
	if (!Data)
	{
		return false;
	}

	mCurrentComboData = Data->WeaponComboData;
	return true;
}

void UGA_DynamicAttack::SetPlayMontage(const FGameplayTagContainer& SubTagContainer)
{
	if (!mCurrentComboData || mPlayNode.IsNone())
	{
		mPlayMontage = nullptr;
		return;
	}

	const FComboNode& CurrentNode = mCurrentComboData->ComboMap[mPlayNode];

	FName NextNodeName;
	if (SubTagContainer.HasTagExact(Sub::Left))
	{
		if (CurrentNode.NextInfo.Contains(EAttackInputType::LeftMouse)) 
		{
			NextNodeName = CurrentNode.NextInfo[EAttackInputType::LeftMouse];
		}
	}
	
	else if (SubTagContainer.HasTagExact(Sub::Right))
	{
		if (CurrentNode.NextInfo.Contains(EAttackInputType::RightMouse))
		{
			NextNodeName = CurrentNode.NextInfo[EAttackInputType::RightMouse];
		}
	}

	mPlayNode = NextNodeName;
	if (mPlayNode.IsNone() || !mCurrentComboData->ComboMap.Contains(mPlayNode))
	{
		mPlayMontage = nullptr;
		return;
	}

	mPlayMontage = mCurrentComboData->ComboMap[mPlayNode].Montage;
	if (mPlayMontage)
	{
		MY_LOG(Error, "PlayMontage %s", *mPlayMontage->GetName());
	}
}

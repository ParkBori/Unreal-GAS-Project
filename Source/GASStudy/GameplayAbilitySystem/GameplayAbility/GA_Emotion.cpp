// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_Emotion.h"

UGA_Emotion::UGA_Emotion()
{
	// 기본태그
	//FGameplayTagContainer DefaultsTags(PlayerAbilityTags::Ability_Dance);
	//SetAssetTags(DefaultsTags);

	//// 활성화 소유 태그 
	//ActivationOwnedTags.AddTag(PlayerAbilityTags::Ability_Dance);

	//// 활성화 차단 태그
	//// 공격, 대쉬, 데미지, 인터렉트, 포션 등을 사용하고 있는 경우 차단해줘야함 
	//TargetBlockedTags;

	//// 트리거 설정
	//FAbilityTriggerData TriggerData;
	//TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	//TriggerData.TriggerTag = PlayerAbilityTags::Ability_Dance;
	//AbilityTriggers.Add(TriggerData);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Dance/Montage/M_Dance.M_Dance'"));
	if (PlayMontageAsset.Succeeded())
	{
		mPlayMontage = PlayMontageAsset.Object;
	}
}

void UGA_Emotion::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	/**
	* TriggerEventData 내부에 존재하는 Event Tag 사용시 검출이 안됨
	* SendGameplayEventActor 의 파라메터로 EventTag 가 있는데,
	* TriggerEventData 에서 Event Tag 가 외부 파라메터 값을 덮이기 때문이다.
	* 별도의 태그 TargTags 나 InstigatorTags 를 사용해야한다.
	*/
	if (TriggerEventData)
	{
		/*bool HasDanceTag = TriggerEventData->TargetTags.HasTag(PlayerAbilityTags::Ability_Dance);
		if (HasDanceTag)
		{
			mSectionName = *TriggerEventData->TargetTags.GetByIndex(0).ToString();
		}*/
	}
}

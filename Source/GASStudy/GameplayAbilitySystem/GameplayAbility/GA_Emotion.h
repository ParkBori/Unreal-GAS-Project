// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_Emotion.generated.h"

/**
 * 춤추는 어빌리티 내부에 여러개의 애니메이션이 있다면 
 * 춤 모션하나마다 각기 다른 어빌리티를 만드는 것은 너무 낭비이다
 * 
 * UGA_Emotion 을 만들고 여기에 플레이 시킬 애니메이션들을 저장한다
 * 
 * 하나의 몽타주를 만들고, 몽타주 섹션이름을 수정할 수 있으면 제일 낫겠다
 */
UCLASS()
class GASSTUDY_API UGA_Emotion : public UGA_PlayMontage
{
	GENERATED_BODY()

public:
	UGA_Emotion();

public:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
};

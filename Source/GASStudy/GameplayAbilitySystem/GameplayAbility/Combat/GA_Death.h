// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_Death.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_Death : public UGA_PlayMontage
{
	GENERATED_BODY()
	
	
public:
	UGA_Death();
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};

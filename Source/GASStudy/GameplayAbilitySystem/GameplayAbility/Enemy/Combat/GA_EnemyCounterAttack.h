// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EnemyCounterAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_EnemyCounterAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_EnemyCounterAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AttackHitCheck();
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	void Callback_OnTraceResult(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void Callback_EndEventReceived(FGameplayEventData Payload);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	float CurrnetComboLevel;

	UPROPERTY()	
	TWeakObjectPtr<class UCombatComponent> mCombatComp;
};

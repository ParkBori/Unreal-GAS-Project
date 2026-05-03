// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_DashDodgeAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_DashDodgeAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_DashDodgeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


private:
	UFUNCTION()
	void OnCompletedCallback();
	UFUNCTION()
	void OnInterruptedCallback();
	UFUNCTION()
	void OnCancelledCallback();

private:
	UPROPERTY(EditAnywhere, meta = (PrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackAnim;
};

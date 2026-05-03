// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_EnemyDodge.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_EnemyDodge : public UGA_PlayMontage
{
	GENERATED_BODY()
	
public:
	UGA_EnemyDodge();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void MontageFinishCallbackFunc();
	
	UAnimMontage* SelectMontage();

protected:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimMontage>> DodgeAnims;
};

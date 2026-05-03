// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_PlayerResurrection.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_PlayerResurrection : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_PlayerResurrection();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	virtual void OnMontageCompletedCallback();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mPlayerDownToUp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> mResurrectionEffect;
};

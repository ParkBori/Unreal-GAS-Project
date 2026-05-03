// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "Shared/Commbat/CombatData.h"
#include "GA_ParriedReaction.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_ParriedReaction : public UGA_Combat// UGA_PlayMontage
{
	GENERATED_BODY()
	
public:
	UGA_ParriedReaction();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UAnimMontage* SelectMontage(const AActor* SourceActor, const FHitResult& HitResult);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ParryedOnMaxPosture;

	UPROPERTY(EditDefaultsOnly, Category = "Anim", meta = (DisplayName = "ParriedReactionMontagesByHitArea"))
	TMap<EHitDirectionArea, TObjectPtr<UAnimMontage>> mParriedReactionMontagesByHitArea;
};


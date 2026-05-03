// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "Shared/Commbat/CombatData.h"
#include "GA_ParryReaction.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_ParryReaction : public UGA_Combat//UGA_PlayMontage
{
	GENERATED_BODY()

public:
	UGA_ParryReaction();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UAnimMontage* SelectMontage(const AActor* SourceActor, const AActor* TargetActor);
	UAnimMontage* SelectMontage(const AActor* SourceActor, const FVector TargetLoaction);
	UAnimMontage* SelectMontage(const AActor* SourceActor, const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly, Category = "Anim", meta = (DisplayName = "ParryReactionMontages"))
	TMap<EHitDirection, TObjectPtr<UAnimMontage>> mParryReactionMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Anim", meta = (DisplayName = "ParryReactionMontagesByHitArea"))
	TMap<EHitDirectionArea, TObjectPtr<UAnimMontage>> mParryReactionMontagesByHitArea;
};

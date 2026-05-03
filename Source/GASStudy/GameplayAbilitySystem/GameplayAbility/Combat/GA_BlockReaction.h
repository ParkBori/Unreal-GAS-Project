// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "Shared/Commbat/CombatData.h"
#include "GA_BlockReaction.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_BlockReaction : public UGA_Combat
{
	GENERATED_BODY()
	
public:
	UGA_BlockReaction();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UAnimMontage* SelectMontage(const AActor* SourceActor, const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Anim", meta = (DisplayName = "BlockReactionMontagesByHitArea"))
	TMap<EHitDirectionArea, TObjectPtr<UAnimMontage>> mBlockReactionMontagesByHitArea;
};

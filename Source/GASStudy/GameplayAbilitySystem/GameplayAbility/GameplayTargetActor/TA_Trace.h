// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ATA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ATA_Trace();

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargeting() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetDataHandle() const;

};

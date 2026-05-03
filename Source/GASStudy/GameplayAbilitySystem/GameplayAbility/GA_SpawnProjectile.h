// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_SpawnProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_SpawnProjectile : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_SpawnProjectile();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
	
protected:
	// 생성될 프로젝타일
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> SpawnProjectile;
};

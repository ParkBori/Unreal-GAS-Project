// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_DrinkPotion.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_DrinkPotion : public UGA_Combat
{
	GENERATED_BODY()

public:
	UGA_DrinkPotion();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	class UInventoryComponent_V2* GetInventory();

protected:
	UFUNCTION()
	void Callback_EventReceived(FGameplayEventData Payload);
};

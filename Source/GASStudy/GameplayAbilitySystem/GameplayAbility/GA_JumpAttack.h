// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interface/GASCanInputInterface.h"
#include "GA_JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_JumpAttack : public UGameplayAbility, public IGASCanInputInterface
{
	GENERATED_BODY()
	
public:
	UGA_JumpAttack();
	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UFUNCTION()
	void OnCompletedCallback();
	UFUNCTION()
	void OnInterruptedCallback();
	UFUNCTION()
	void OnCancelledCallback();

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);
	
	virtual void SetCanInput(bool InCanInput) override;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FName StartSection;
	
	FName CurrentSection;

private:
	UPROPERTY(EditAnywhere, meta = (PrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackAnim;
	
};

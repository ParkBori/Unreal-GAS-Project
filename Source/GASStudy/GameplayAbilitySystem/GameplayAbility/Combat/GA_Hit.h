// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_Hit.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_Hit : public UGA_Combat
{
	GENERATED_BODY()
	
	public:
		UGA_Hit();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UAnimMontage* SelectHitMontage(const AActor* Vicitim, const AActor* Instigator);

	bool CheckMaxPosture();

protected: 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mRightHit;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mLeftHit;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mForwardHit;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mBackHit;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mHeavyAttack;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> HitOnMaxPostureState_V1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> HitOnMaxPostureState_V2;
};

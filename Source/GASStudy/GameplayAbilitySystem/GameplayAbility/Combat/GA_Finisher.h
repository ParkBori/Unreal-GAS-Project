// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_Finisher.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_Finisher : public UGA_Combat
{
	GENERATED_BODY()
	
public:
	UGA_Finisher();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	bool CheckPlayerIsTargetBack();

protected:
	bool IsPlayerBack;

	UPROPERTY(EditAnywhere)
	TObjectPtr< UAnimMontage> mFront;

	UPROPERTY(EditAnywhere)
	TObjectPtr< UAnimMontage> mBack;

	UPROPERTY(EditAnywhere)
	FVector mFrontLocationOffset;
	UPROPERTY(EditAnywhere)
	FRotator mFrontRotationOffset;
	UPROPERTY(EditAnywhere)
	FVector mBackLocationOffset;
	UPROPERTY(EditAnywhere)
	FRotator mBackRotationOffset;

	bool bFirstActive = true;
};

UCLASS()
class GASSTUDY_API UGA_Finished: public UGA_Combat
{
	GENERATED_BODY()

public:
	UGA_Finished();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UAnimMontage* SelectMontage();

protected:
	bool IsPlayerBack;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mFront;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> mBack;
};

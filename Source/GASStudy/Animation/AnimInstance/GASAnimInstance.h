// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Shared/Commbat/CombatData.h"
#include "GASAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGASAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float mDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldStart;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldStop;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float mGroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector mVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|Block")
	bool bIsBlocking;

	// 블럭 자세 여부 
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Block")
	EBlockPosition mCurrentBlockPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ARPGCharacter> mCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character|Component")
	TWeakObjectPtr<class UCombatComponent> mCombatComp;

	UPROPERTY(BlueprintReadOnly, Category = "Character|ASC")
	TWeakObjectPtr<class UAbilitySystemComponent> mASC;

	UPROPERTY(BlueprintReadOnly, Category = "Character|Component")
	TWeakObjectPtr<class UCharacterMovementComponent> mMovementComp;
};

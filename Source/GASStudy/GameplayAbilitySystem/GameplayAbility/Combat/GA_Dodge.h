// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "Shared/Commbat/CombatData.h"
#include "GA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_Dodge : public UGA_Combat
{
	GENERATED_BODY()
	
public:
	UGA_Dodge();

public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* SelectMontage();
	virtual UAnimMontage* SelectMontage_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RePressed();
	virtual void RePressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetRotation(bool IsPlus);
	virtual void SetRotation_Implementation(bool IsPlus);

protected:
	UPROPERTY(EditAnywhere, Category = "Dodge|Anim", BlueprintReadWrite, meta = (DisplayName = "DodgeMotnages"))
	TMap<EHitDirection, TObjectPtr<UAnimMontage>> mDodgeMontages;

	UPROPERTY(EditAnywhere, Category = "Dodge|Anim", BlueprintReadWrite, meta = (DisplayName = "RollMontages"))
	TMap<EHitDirection, TObjectPtr<UAnimMontage>> mRollMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge|Direction", meta = (DisplayName = "Direction"))
	EHitDirection mCalculatedDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (DisplayName = "RePressed"))
	bool bRePressed = false;
};

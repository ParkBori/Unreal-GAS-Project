// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_ToggleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGA_ToggleWeapon : public UGA_PlayMontage
{
	GENERATED_BODY()
	
	

public:
	UGA_ToggleWeapon();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

private:
	UAnimMontage* SetPlayMontage(bool IsWeaponActiveed, const FGameplayTag& EquipWeaponTag);


protected:
	// 몽타주 재생 관련 콜백함수
	// virtual void Callback_OnCompletedMontage() override;
	// virtual void Callback_OnInterruptedMontage() override;
	// virtual void Callback_OnCancelledMontage() override;

	UPROPERTY(EditAnywhere, Category ="EquipAnimation")
	TObjectPtr<UDataTable> mEquipAnimDT;

};

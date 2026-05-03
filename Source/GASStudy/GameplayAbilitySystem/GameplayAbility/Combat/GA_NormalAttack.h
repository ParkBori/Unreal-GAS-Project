// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_NormalAttack.generated.h"

/**
 * 플레이어 노말 어택
 */
UCLASS()
class GASSTUDY_API UGA_NormalAttack : public UGA_Combat
{
	GENERATED_BODY()
	
public:
	UGA_NormalAttack();
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	bool CanExecuteFinisher();

private:
	/** Utill */
	bool SetComboData(const FString& WeaponTag);

	UAnimMontage* SetPlayMontage();

	FString GetLastTagName(const FGameplayTag& Tag);

	bool IsBehindTarget(AActor* Target);


private:
	int32 mCurrentCombo = 0;
	int32 mMaxCombo = 0;

	FVector mFwdCheckOffset = FVector(200.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UDataTable> mAnimDT;

	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> mPlayList;

	UPROPERTY()
	TWeakObjectPtr<class UEquipComponent> mEquipComponent;
};

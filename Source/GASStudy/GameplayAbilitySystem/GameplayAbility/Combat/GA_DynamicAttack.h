// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_DynamicAttack.generated.h"

/**
 * 서브 태그를 받아서, 다양한 형태의 몽타주 단위로 분기러기 가능한 어빌리티
 * 
 */
UCLASS()
class GASSTUDY_API UGA_DynamicAttack : public UGA_PlayMontage
{
	GENERATED_BODY()
	
public:
	UGA_DynamicAttack();
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;




private:
	UFUNCTION()
	void Callback_EventReceived(FGameplayEventData Payload);

private:
	/** Utill */
	bool SetComboData(const FName& Key);

	void SetPlayMontage(const FGameplayTagContainer& SubTagContainer);

private:
	bool bFirst = true;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UDataTable> mAnimDT;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TObjectPtr<class UWeaponComboData> mCurrentComboData;

	UPROPERTY()
	TWeakObjectPtr<class UEquipComponent> mEquipComponent;

	FName mPlayNode;
};

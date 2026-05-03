// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Shared/Tag/SharedTag.h"
#include "Shared/GAS/GASData.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Shared/Macro/Log.h"
#include "Component/CombatComponent.h"
#include "GA_Base.generated.h"

/**
 * 기본 GA 어빌리티
 * 기본 세팅 정의
 * 
 * 에셋 / 활성화는 동일한 태그를 가지며, 1가지만 소유할 수 있다
 * 이벤트 트리거 태그도 하나만 소유하게 한다
 */
USTRUCT()
struct FSetByCallerData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag DataTag;
	
	UPROPERTY()
	float Value = 0.f;
};

UCLASS()
class GASSTUDY_API UGA_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

protected:
	void ApplySetByCallerEffect(TSubclassOf<UGameplayEffect> EffectClass, const TArray<FSetByCallerData>& SetByCallerData);
};

UCLASS()
class GASSTUDY_API UGA_PlayMontage : public UGA_Base
{
	GENERATED_BODY()

public:
	UGA_PlayMontage();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	/**
	* 어빌리티 태스크를 사용한 몽타주 재생 함수
	*/
	UFUNCTION(BlueprintCallable)
	virtual void PlayMontage(UAnimMontage* MontageToPlay);

protected:
	// 몽타주 재생 관련 콜백함수
	UFUNCTION()
	virtual void Callback_OnCompletedMontage();
	UFUNCTION()
	virtual void Callback_OnInterruptedMontage();
	UFUNCTION()
	virtual void Callback_OnCancelledMontage();

protected:
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> mAT_PlayMontageTask;

	UPROPERTY(BlueprintReadWrite, Category = "PlayAnimation", EditDefaultsOnly, meta = (PrivateAccess = true, DisplayAfter = "PlayMontage"))
	TObjectPtr<UAnimMontage> mPlayMontage;

	UPROPERTY(Category = "PlayAnimation", EditDefaultsOnly, meta = (PrivateAccess = true))
	float PlayRate = 1.f;

	UPROPERTY(Category = "PlayAnimation", EditDefaultsOnly, meta = (PrivateAccess = true))
	FName mSectionName;

	int32 mMontageLevel;
};

UCLASS()
class GASSTUDY_API UGA_Combat : public UGA_PlayMontage
{
	GENERATED_BODY()

public:
	UGA_Combat();

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	virtual void PlayMontage(UAnimMontage* MontageToPlay) override;

private:
	UFUNCTION()
	void Callback_AddedCancelWindowTag();

	UFUNCTION()
	void Callback_RemovedCancelWindowTag();

private:
	UPROPERTY()
	FGameplayTagContainer mAlwaysExcute;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (DisplayName = "IsAttackAbility"))
	bool bIsAttackAbility;

	// CurveTable, DataTable의 키로 탐색할 스킬 이름
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (DisplayName = "CombatAbilityName"))
	FName mAttackName;

	// 어빌리티에 해당하는 밸런스 정보
	FGASAttackAbilityData* mAttackDataRow;
	
	UPROPERTY()
	TWeakObjectPtr<UCombatComponent> mCombatComp;
};


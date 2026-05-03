// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PlayMontage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageWaitSimpleDelegate_1);


/**
 * 
 */
UCLASS()
class GASSTUDY_API UAT_PlayMontage : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	// 현재 GT 의 OwningAbility  가 설정하는 델리게이트
	// 즉 외부에서 이 델리게이트를 사용함
	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate_1	OnCompleted;
	
	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate_1	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate_1	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate_1	OnCancelled;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMontageAndWait",
	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_PlayMontage* Create(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false);


	virtual void Activate() override;


	bool PlayMontage();

	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ChangeMontage(UAnimMontage* NewMontage);

protected:
	// 재생 몽타주 전용 델리게이트 
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	// GT 내부적으로 GT 를 종료시키기위한 핸들 
	FDelegateHandle InterruptedHandle;


	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float StartTimeSeconds;

	void SetMontageDelegate();


};

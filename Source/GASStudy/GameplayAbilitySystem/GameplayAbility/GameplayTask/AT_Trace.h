// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_Trace.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&,TargetDataHandle);
/**
 * 
 */
UCLASS()
class GASSTUDY_API UAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAT_Trace();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ATA_Trace> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	void Callback_TargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle);

	// 생성할 클래스 타입 
	UPROPERTY()
	TSubclassOf<class ATA_Trace> TargetActorClass;

	// 생성할 클래스로 실제 생성되는 액터
	UPROPERTY()
	TObjectPtr<class ATA_Trace> mSpawnedTargetActor;
};

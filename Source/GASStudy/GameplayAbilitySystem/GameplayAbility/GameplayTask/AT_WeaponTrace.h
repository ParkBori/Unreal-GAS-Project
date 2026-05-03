// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WeaponTrace.generated.h"

class ATA_WeaponTrace;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetDataReady, const FGameplayAbilityTargetDataHandle&);
/**
 * 
 */
UCLASS()
class GASSTUDY_API UAT_WeaponTrace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAT_WeaponTrace();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WeaponTrace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_WeaponTrace> TargetActorClass, const FGameplayEventData& TriggerEventData);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();


protected:
	void Callback_TargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle);

private:
	// 생성할 클래스 타입 
	UPROPERTY()
	TSubclassOf<ATA_WeaponTrace> mTargetActorClass;

	// 생성할 클래스로 실제 생성되는 액터
	UPROPERTY()
	TObjectPtr<ATA_WeaponTrace> mSpawnedTargetActor;

	struct FGameplayAbilityTargetData_WeaponTraceData* mData;

public:
	FOnTargetDataReady mOnTargetDataReady;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_WeaponTrace.generated.h"

/**
 * 
 */



DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetDataReady, const FGameplayAbilityTargetDataHandle&);


UCLASS()
class GASSTUDY_API ATA_WeaponTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ATA_WeaponTrace();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;

protected:
	void MakeTargetDataHandle(const TArray<FHitResult>& HitResults) const;

private:
	UPROPERTY()
	TWeakObjectPtr<class UWeaponTraceComponent> mWeaponTraceComp;

	struct FGameplayAbilityTargetData_WeaponTraceData* Data;


public:
	FOnTargetDataReady mOnTargetDataReady;

	friend class UAT_WeaponTrace;
};

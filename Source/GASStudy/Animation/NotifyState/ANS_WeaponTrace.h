// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Shared/Weapon/WeaponData.h"
#include "ANS_WeaponTrace.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UANS_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditInstanceOnly, Category = "WeaponTracData")
	float mNotifyStartTime;
	
	UPROPERTY(EditInstanceOnly, Category = "WeaponTracData")
	FName mStartSocket;

	UPROPERTY(EditInstanceOnly, Category = "WeaponTracData")
	FName mEndSocket;

	UPROPERTY(EditInstanceOnly, Category = "WeaponTracData")
	EWeaponTraceShape mWeaponTraceShape;
};


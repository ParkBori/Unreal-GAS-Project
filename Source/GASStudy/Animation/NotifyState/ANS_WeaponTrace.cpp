// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_WeaponTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Shared/Tag/SharedTag.h"
#include "Shared/GAS/GASData.h"




void UANS_WeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			FGameplayAbilityTargetData_WeaponTraceData* Data = new FGameplayAbilityTargetData_WeaponTraceData(mStartSocket, mEndSocket, *Animation->GetName(), mNotifyStartTime, mWeaponTraceShape);
			PayloadData.TargetData.Add(Data);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, Event::Event_StartAttackHitCheck, PayloadData);
		}
	}
}

void UANS_WeaponTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			// PayloadData.EventMagnitude = ComboLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, Event::Event_EndAttackHitCheck, PayloadData);
		}
	}
}

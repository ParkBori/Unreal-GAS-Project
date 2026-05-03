// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_RotateToTarget.h"
#include "Component/EnemyCombatComponent.h"

void UANS_RotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	mEnemyCombatComp = OwnerActor->FindComponentByClass<UEnemyCombatComponent>();
	if (mEnemyCombatComp.IsValid())
	{
		mEnemyCombatComp->ActivateFocus(false);
	}
}

void UANS_RotateToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!mEnemyCombatComp.IsValid())
	{
		return;
	}

	mEnemyCombatComp->RotateToTarget();
}

//void UANS_RotateToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
//{
//	Super::NotifyEnd(MeshComp, Animation, EventReference);
//
//	if (mEnemyCombatComp.IsValid())
//	{
//		mEnemyCombatComp->ActivateFocus(true);
//	}
//}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_RotateWhileAttacking.h"
#include "Interface/CombatInterface.h"

void UANS_RotateWhileAttacking::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);


}

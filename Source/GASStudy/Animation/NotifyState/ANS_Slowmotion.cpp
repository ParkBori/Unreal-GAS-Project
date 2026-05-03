// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_Slowmotion.h"
#include "Kismet/GameplayStatics.h"
void UANS_Slowmotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (IsValid(OwnerActor))
	{
		if (UWorld* World = OwnerActor->GetWorld())
		{
			UGameplayStatics::SetGlobalTimeDilation(World, SlowmotionSpeed);
		}
	}
}

void UANS_Slowmotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (IsValid(OwnerActor))
	{
		if (UWorld* World = OwnerActor->GetWorld())
		{
			UGameplayStatics::SetGlobalTimeDilation(World, 1);
		}
	}
}

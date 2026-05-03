// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_AfterEffect.h"
#include "Effect/AfterEffect.h"
#include "Components/CapsuleComponent.h"

void UANS_AfterEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Owner = MeshComp->GetOwner();
	if (IsValid(Owner))
	{
		SpawnEffect(MeshComp);
	}
}

void UANS_AfterEffect::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(Owner))
	{
		//SpawnEffect(MeshComp);
	}
}

void UANS_AfterEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(Owner))
	{
		//SpawnEffect(MeshComp);
	}
}

void UANS_AfterEffect::SpawnEffect(USkeletalMeshComponent* MeshComp)
{
	UWorld* World = MeshComp->GetWorld();
	
	if (IsValid(Owner) && IsValid(World))
	{
		UCapsuleComponent* Capsule = Owner->FindComponentByClass<UCapsuleComponent>();
		if (IsValid(Capsule))
		{
			FVector Location = Owner->GetActorLocation();
			Location.Z -= Capsule->GetScaledCapsuleHalfHeight();
			AAfterEffect* AfterEffect = World->SpawnActor<AAfterEffect>(AAfterEffect::StaticClass(), Location, Owner->GetActorRotation());
			AfterEffect->Init(MeshComp);
		}
	}
}

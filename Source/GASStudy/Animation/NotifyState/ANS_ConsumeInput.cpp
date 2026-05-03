// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_ConsumeInput.h"
#include "Component/PlayerCombatComponent.h"

void UANS_ConsumeInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	//AActor* Owner = MeshComp->GetOwner();
	//if (Owner && Owner->ActorHasTag(TEXT("Player")))
	//{
	//	if (UCombatComponent* CC = Owner->FindComponentByClass<UCombatComponent>())
	//	{
	//		CC->ConsumeInput();
	//	}
	//}
}

void UANS_ConsumeInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

 	AActor* Owner = MeshComp->GetOwner();
	if (Owner && Owner->ActorHasTag(TEXT("Player")))
	{
		if (UPlayerCombatComponent* CC = Owner->FindComponentByClass<UPlayerCombatComponent>())
		{
			CC->ConsumeInput();
		}
	}
}

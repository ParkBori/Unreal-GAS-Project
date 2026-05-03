// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AN_GATrigger.h"
#include "AbilitySystemBlueprintLibrary.h"


UAN_GATrigger::UAN_GATrigger()
{
}

void UAN_GATrigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = AbilityAEndTime;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AN_RemoveGameplayTag.h"
#include "AbilitySystemComponent.h"


void UAN_RemoveGameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	auto* ASC = OwnerActor->GetComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTags(mRemoveTags);
	}
}

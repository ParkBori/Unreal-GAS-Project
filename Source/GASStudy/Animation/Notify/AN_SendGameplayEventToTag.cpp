// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AN_SendGameplayEventToTag.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAN_SendGameplayEventToTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, mEventTag, FGameplayEventData());
}

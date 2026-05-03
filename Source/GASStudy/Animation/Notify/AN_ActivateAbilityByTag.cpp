// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AN_ActivateAbilityByTag.h"
#include "AN_ActivateAbilityByTag.h"
#include "AbilitySystemBlueprintLibrary.h"

UAN_ActivateAbilityByTag::UAN_ActivateAbilityByTag()
{
}

void UAN_ActivateAbilityByTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// TODO : 설정된 태그로 어빌리티 발동 
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (IsValid(OwnerActor))
	{
		//if (OwnerActor->Tags.Contains(TEXT("Enemy")))
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, AtivateAbilityTriggerTag, FGameplayEventData());
		}
	}
}

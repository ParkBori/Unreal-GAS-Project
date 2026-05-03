// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_Parry.h"
#include "AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"



void UANS_Parry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (OwnerASC)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = OwnerASC->MakeOutgoingSpec(StartEffect, 1, OwnerASC->MakeEffectContext());
		if (EffectSpecHandle.IsValid())
		{
			OwnerASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		}
	}
}

void UANS_Parry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (OwnerASC)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = OwnerASC->MakeOutgoingSpec(EndEffect, 1, OwnerASC->MakeEffectContext());
		if (EffectSpecHandle.IsValid())
		{
			OwnerASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.
#include "Animation/NotifyState/ANS_AddGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Shared/Tag/SharedTag.h"

void UANS_AddGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	if (mAddTagCharacterType == ECharacterType::Player)
	{
		if (!OwnerActor->ActorHasTag("Player"))
		{
			return;
		}
	}
	else if (mAddTagCharacterType == ECharacterType::NonPlayer)
	{
		if (!OwnerActor->ActorHasTag("Enemy"))
		{
			return;
		}
	}
	
	mASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (mASC.IsValid())
	{
		for (const FGameplayTag& Tag : mAddTags.GetGameplayTagArray())
		{
			mASC->AddLooseGameplayTag(Tag);
		}
	}
}

void UANS_AddGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (mASC.IsValid())
	{
		for (const FGameplayTag& Tag : mAddTags.GetGameplayTagArray())
		{
			mASC->RemoveLooseGameplayTag(Tag);
		}
	}
}

UANS_CancelWindow::UANS_CancelWindow()
{
	mAddTagCharacterType = ECharacterType::All;
	mAddTags.AddTag(State::State_CancelWindow);
	mAddTags.AddTag(State::State_Cancelable);
}

UANS_AttackActivate::UANS_AttackActivate()
{
	mAddTagCharacterType = ECharacterType::All;
	mAddTags.AddTag(State::State_Attack_Active);
}

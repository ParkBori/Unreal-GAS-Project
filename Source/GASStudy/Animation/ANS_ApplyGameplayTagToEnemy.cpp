// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_ApplyGameplayTagToEnemy.h"

#include "Interface/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tag/GameplayTag.h"

UANS_ApplyGameplayTagToEnemy::UANS_ApplyGameplayTagToEnemy()
{

}

void UANS_ApplyGameplayTagToEnemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// TODO :
	// 플레이어의 현재 타겟 액터를 받아와서, 이액터에게 블럭 가능함을 알려줌
	// 문제점
	// 애니메이션이 겹치는게 있을 수 있는데, 오너가 Enemy 일때가 아니라 플레이어가 해당애니메이션을 재생시킬때임 

	AActor* OwnerActor = MeshComp->GetOwner();
	if (nullptr == OwnerActor)
	{
		return;
	}
	// 이런식으로 플레이어인지 타겟인지 확인가ㅡㅇ 
	bool IsPlayer = OwnerActor->Tags.Contains(TEXT("Player"));

	if (IsPlayer)
	{
		ICombatInterface* CharacterUtilInterface = Cast<ICombatInterface>(OwnerActor);
		if (CharacterUtilInterface)
		{
			AActor* Target = CharacterUtilInterface->GetTarget();
			if (Target)
			{
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
				TargetASC->AddLooseGameplayTag(ApplyGameplayTag);
			}
		}
	}
}

void UANS_ApplyGameplayTagToEnemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// TODO :
	// 플레이어의 현재 타겟 액터를 받아와서, 이액터에게 블럭 가능함을 알려줌
	// 문제점
	// 애니메이션이 겹치는게 있을 수 있는데, 오너가 Enemy 일때가 아니라 플레이어가 해당애니메이션을 재생시킬때임 

	AActor* OwnerActor = MeshComp->GetOwner();
	if (nullptr == OwnerActor)
	{
		return;
	}
	// 이런식으로 플레이어인지 타겟인지 확인가ㅡㅇ 
	bool IsPlayer = OwnerActor->Tags.Contains(TEXT("Player"));

	if (IsPlayer)
	{
		ICombatInterface* CharacterUtilInterface = Cast<ICombatInterface>(OwnerActor);
		if (CharacterUtilInterface)
		{
			AActor* Target = CharacterUtilInterface->GetTarget();
			if (Target)
			{
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
				TargetASC->RemoveLooseGameplayTag(ApplyGameplayTag);
			}
		}
	}
}

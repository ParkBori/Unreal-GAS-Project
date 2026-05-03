// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_CanCounter.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tag/GameplayTag.h"


void UANS_CanCounter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	// TODO :
	// 플레이어의 현재 타겟 액터를 받아와서, 이액터에게 블럭 가능함을 알려줌
	// 문제점
	// 애니메이션이 겹치는게 있을 수 있는데, 오너가 Enemy 일때가 아니라 플레이어가 해당애니메이션을 재생시킬때임 

	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor)
	{
		bool IsPlayer = OwnerActor->Tags.Contains(TEXT("Player"));
		if (IsPlayer)
		{
			ICombatInterface* CharacterUtilInterface = Cast<ICombatInterface>(OwnerActor);
			if (CharacterUtilInterface)
			{
				AActor* Target = CharacterUtilInterface->GetTarget();
				if (Target)
				{
					ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
					if (ASC.IsValid())
					{
						ASC->AddLooseGameplayTag(TAG_STATE_CANCOUNTER);
					}
				}
			}
		}
	}
}

//void UANS_EnemyCanBlock::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
//{
//	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
//}

void UANS_CanCounter::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (ASC.IsValid())
	{
		ASC->RemoveLooseGameplayTag(TAG_STATE_CANCOUNTER);
	}
}

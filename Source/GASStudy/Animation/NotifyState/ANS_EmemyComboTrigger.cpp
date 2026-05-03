// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_EmemyComboTrigger.h"
#include "Character/NonPlayable/Enemy/Enemy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/GameplayTag.h"

void UANS_EmemyComboTrigger::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (IsValid(Owner))
	{
		bool IsEnemy = Owner->Tags.Contains(TEXT("Enemy"));
		if (IsEnemy)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, TAG_EVENT_ATTACKREINPUT, FGameplayEventData());
		}
		
		/*AEnemy* Enemy = Cast<AEnemy>(Owner);
		if (IsValid(Enemy))
		{
			UE_LOG(LogTemp, Warning, TEXT("ComboTrigger"));
			Enemy->SetCanNextCombo(true);
		}*/
	}
}




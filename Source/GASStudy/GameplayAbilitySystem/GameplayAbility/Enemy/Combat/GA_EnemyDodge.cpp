// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Enemy/Combat/GA_EnemyDodge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
UGA_EnemyDodge::UGA_EnemyDodge()
{
}

void UGA_EnemyDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// TODO : 닷지 애니메이션 재생 
	UAnimMontage* SelectedMontage = SelectMontage();
	if (nullptr == SelectedMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Montage"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}


	AActor* OwnerActor = ActorInfo->AvatarActor.Get();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwnerActor);
	if (CombatInterface)
	{
		AActor* TargetActor = CombatInterface->GetTarget();
		if (IsValid(TargetActor))
		{
			FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), TargetActor->GetActorLocation());
			OwnerActor->SetActorRotation(TargetRot);
		}
	}

	UAbilityTask_PlayMontageAndWait* AT_PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDoDgeAnim"), SelectedMontage);
	AT_PlayMontage->OnCompleted.AddDynamic(this, &UGA_EnemyDodge::MontageFinishCallbackFunc);
	AT_PlayMontage->OnBlendOut.AddDynamic(this, &UGA_EnemyDodge::MontageFinishCallbackFunc);
	AT_PlayMontage->OnCancelled.AddDynamic(this, &UGA_EnemyDodge::MontageFinishCallbackFunc);
	AT_PlayMontage->OnInterrupted.AddDynamic(this, &UGA_EnemyDodge::MontageFinishCallbackFunc);
	//AT_PlayMontage->OnCancelled.AddDynamic(this, &UGA_EnemyDodge::MontageFinishCallbackFunc);
	AT_PlayMontage->ReadyForActivation();
}

void UGA_EnemyDodge::MontageFinishCallbackFunc()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

UAnimMontage* UGA_EnemyDodge::SelectMontage()
{
	// 몽타주 배열에서 애니메이션 선택 
	int ArrNum = DodgeAnims.Num();
	if (0 == ArrNum)
	{
		return nullptr;
	}

	int32 Idx = FMath::Rand32() % ArrNum;
	return DodgeAnims[Idx];
}

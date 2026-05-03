// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GameplayTask/AT_Trace.h"
#include "GameplayAbilitySystem/GameplayAbility/GameplayTargetActor/TA_Trace.h"
#include "AbilitySystemComponent.h"


UAT_Trace::UAT_Trace()
{
}

UAT_Trace* UAT_Trace::CreateTask(UGameplayAbility* OwningAbility,  TSubclassOf<ATA_Trace> TargetActorClass)
{
	// TargetActorClass 는 생성할 클래스타입임
	UAT_Trace* NewTask = NewAbilityTask<UAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UAT_Trace::Activate()
{
	Super::Activate();
	
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	if (mSpawnedTargetActor)
	{
		mSpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAT_Trace::SpawnAndInitializeTargetActor()
{
	mSpawnedTargetActor = Cast<ATA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (mSpawnedTargetActor)
	{
		//mSpawnedTargetActor->SetShowDebug(true);
		mSpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_Trace::Callback_TargetDataReady);
	}
}

void UAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		mSpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(mSpawnedTargetActor);
		mSpawnedTargetActor->StartTargeting(Ability);
		mSpawnedTargetActor->ConfirmTargeting();
	}
}

void UAT_Trace::Callback_TargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GameplayTask/AT_WeaponTrace.h"
#include "GameplayAbilitySystem/GameplayAbility/GameplayTargetActor/TA_WeaponTrace.h"
#include "AbilitySystemComponent.h"
#include "Shared/GAS/GASData.h"

UAT_WeaponTrace::UAT_WeaponTrace()
{
}

UAT_WeaponTrace* UAT_WeaponTrace::CreateTask(UGameplayAbility* OwningAbility,  TSubclassOf<ATA_WeaponTrace> TargetActorClass, const FGameplayEventData& TriggerEventData)
{
	// TargetActorClass 는 생성할 클래스타입임
	UAT_WeaponTrace* NewTask = NewAbilityTask<UAT_WeaponTrace>(OwningAbility);
	NewTask->mTargetActorClass = TargetActorClass;
	NewTask->mData = (FGameplayAbilityTargetData_WeaponTraceData*)(TriggerEventData.TargetData.Get(0));

	return NewTask;
}

void UAT_WeaponTrace::Activate()
{
	Super::Activate();
	
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UAT_WeaponTrace::OnDestroy(bool bInOwnerFinished)
{
	if (mSpawnedTargetActor)
	{
		mSpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAT_WeaponTrace::SpawnAndInitializeTargetActor()
{
	if (!mTargetActorClass)
	{
		return;
	}

	mSpawnedTargetActor = Ability->GetWorld()->SpawnActorDeferred<ATA_WeaponTrace>(mTargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (mSpawnedTargetActor)
	{
		//mSpawnedTargetActor->SetShowDebug(true);
		mSpawnedTargetActor->Data = mData;
		mSpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_WeaponTrace::Callback_TargetDataReady);
	}
}

void UAT_WeaponTrace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		mSpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(mSpawnedTargetActor);
		mSpawnedTargetActor->StartTargeting(Ability);
	}
}

void UAT_WeaponTrace::Callback_TargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		mOnTargetDataReady.Broadcast(DataHandle);
	}
}

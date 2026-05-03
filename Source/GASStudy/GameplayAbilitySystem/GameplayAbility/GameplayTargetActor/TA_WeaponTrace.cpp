// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GameplayTargetActor/TA_WeaponTrace.h"
#include "AbilitySystemComponent.h"

#include "Component/WeaponTraceComponent.h"
#include "Shared/GAS/GASData.h"

ATA_WeaponTrace::ATA_WeaponTrace()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

void ATA_WeaponTrace::Tick(float DeltaSeconds) 
{
	// 충돌 판정 및 전달 
	mWeaponTraceComp->Trace();
}

void ATA_WeaponTrace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	mWeaponTraceComp->Reset();
	mWeaponTraceComp->mOnDetecedHit.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void ATA_WeaponTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor =  Ability->GetCurrentActorInfo()->AvatarActor.Get();

	mWeaponTraceComp = SourceActor->GetComponentByClass<UWeaponTraceComponent>();
	if (!mWeaponTraceComp.IsValid())
	{
		return;
	}
	mWeaponTraceComp->mOnDetecedHit.AddUObject(this, &ATA_WeaponTrace::MakeTargetDataHandle);
	mWeaponTraceComp->Init(SourceActor, Data->mStartSocket, Data->mEndSocket, Data->mWeaponTraceShape);
	mWeaponTraceComp->SetTraceData(Data->mNotifyStartTime, Data->mAnimName);

}


void ATA_WeaponTrace::MakeTargetDataHandle(const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle Handle;

	for (const auto& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		Handle.Add(TargetData);
	}

	TargetDataReadyDelegate.Broadcast(Handle);
}


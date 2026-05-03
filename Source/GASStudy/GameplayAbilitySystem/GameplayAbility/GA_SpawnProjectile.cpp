// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_SpawnProjectile.h"
#include "Projectile/BaseProjectile.h"

UGA_SpawnProjectile::UGA_SpawnProjectile()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGA_SpawnProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Cost 나 CoolDown 이 존재한다면,  Commity Ability 직접 호출 필요 
	//if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	//{
	//	constexpr bool bReplicateEndAbility = true;
	//	constexpr bool bWasCancelled = true;
	//	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//}


	UWorld* World = GetAvatarActorFromActorInfo()->GetWorld();
	if (false == IsValid(World))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 발사체 생성 
	
	if (false == ActorInfo->SkeletalMeshComponent.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* Spawner = GetAvatarActorFromActorInfo();
	if (nullptr == Spawner)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector SpawnLocation = Spawner->GetActorLocation() + Spawner->GetActorForwardVector() * 430.f;


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Spawner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 아래는 소환된 액터거 데미지를 입힐경우 
	// 누가 책임자가 되는지인데, 폰으로 받아야해서 일단은 생략
	// 또한 내가 직접 오너를 지정해주는데..
	// 이거는 오버랩 이벤트 발생했을때 한번 따로 내부에서 확인이 되는지 체크가 필요함
	
	// SpawnParams.Instigator = Spawner;


	FRotator SpawnRot = Spawner->GetActorRotation();
	SpawnRot.Pitch = 0;
	SpawnRot.Roll = 0;

	ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(SpawnProjectile, SpawnLocation, SpawnRot, SpawnParams);
	Projectile->FireInDirection(Spawner);
	
	DrawDebugSphere(GetWorld(), SpawnLocation, 30, 10, FColor::Black, false, 30);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

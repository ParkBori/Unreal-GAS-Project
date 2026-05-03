// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GameplayTargetActor/TA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "AbilitySystemBlueprintLibrary.h"

ATA_Trace::ATA_Trace()
{
}

void ATA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor =  Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ATA_Trace::ConfirmTargeting()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetDataHandle();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ATA_Trace::MakeTargetDataHandle() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	const UCharacterAttributeSet* AttributeSet = ASC->GetSet<UCharacterAttributeSet>();
	if (!AttributeSet)
	{
		return FGameplayAbilityTargetDataHandle();
	}
	
	FHitResult HitResult;
	float AttackRange = 0;
	float AttackRadius = 0;

	FVector Fwd = Character->GetActorForwardVector();
	FVector Start = Character->GetActorLocation() + (Fwd * Character->GetCapsuleComponent()->GetScaledCapsuleRadius());
	FVector End = Character->GetActorLocation() + (Fwd * AttackRange);
	FCollisionObjectQueryParams CollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Trace), false, SourceActor);


	bool Result = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(50.0f), CollisionQueryParams);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	
	if (Result)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);
	}

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = Result ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Fwd).ToQuat(), DrawColor, false, 5.0f);

	return DataHandle;
}

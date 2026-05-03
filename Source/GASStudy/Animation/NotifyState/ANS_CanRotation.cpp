// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_CanRotation.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_CanRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		//OwnerCharacter->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}

void UANS_CanRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		FRotator ControllerRotation = OwnerCharacter->GetControlRotation();
		FRotator NewRotation = FRotator(0.f, ControllerRotation.Yaw, 0.f); // Pitch와 Roll 제외
		OwnerCharacter->SetActorRotation(NewRotation);
		//OwnerCharacter->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}

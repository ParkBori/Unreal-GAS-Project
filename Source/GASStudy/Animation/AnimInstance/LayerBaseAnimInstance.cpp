// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/LayerBaseAnimInstance.h"
#include "Animation/AnimInstance/GASAnimInstance.h"

void ULayerBaseAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* Pawn = TryGetPawnOwner();
    if (!Pawn)
    {
        return;
    }

    USkeletalMeshComponent* Mesh = Pawn->FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh)
    {
        return;
    }

    mBaseAnim = Cast<UGASAnimInstance>(Mesh->GetAnimInstance());
}


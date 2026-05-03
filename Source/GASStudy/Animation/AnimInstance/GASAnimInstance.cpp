// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/GASAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "KismetAnimationLibrary.h"

#include "Character/RPGCharacter.h"
#include "Component/CombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Shared/Tag/SharedTag.h"

void UGASAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* Pawn = TryGetPawnOwner();
    if (!Pawn)
    {
        return;
    }

    mCharacter = Cast<ARPGCharacter>(Pawn);
    if (!mCharacter)
    {
        return;
    }

    mCombatComp = mCharacter->FindComponentByClass<UCombatComponent>();
    if (!mCombatComp.IsValid())
    {
        return;
    }

    mMovementComp = mCharacter->GetCharacterMovement();
    if (!mMovementComp.IsValid())
    {
        return;
    }

    mASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(mCharacter);
}

void UGASAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!mMovementComp.IsValid())
    {
        return;
    }

    mVelocity = mMovementComp->Velocity;
    
    // 지면 스피드 
    mGroundSpeed = mVelocity.Size2D();

    //mDirection = /*UKismetAnimationLibrary::*/CalculateDirection(mVelocity, mCharacter->GetActorRotation());
    mDirection = UKismetAnimationLibrary::CalculateDirection(mVelocity, mCharacter->GetActorRotation());

    // 떨어지는지 확인
    bIsFalling= mMovementComp->IsFalling();

    // 가속 여부
    FVector Accel = mMovementComp->GetCurrentAcceleration();
    bIsAccelerating = Accel.Size() > 0.f;

    // Start 조건
    bShouldStart = (mGroundSpeed > 50.f && bIsAccelerating);

    // Stop 조건
    bShouldStop = (/*mGroundSpeed < 10.f && */!bIsAccelerating);

    // 전투 관련 로직
    if (mCombatComp.IsValid() && mASC.IsValid())
    {
        FGameplayTagContainer Tags = mASC->GetOwnedGameplayTags();
        if (Tags.HasTagExact(State::State_Blocking))
        {
            bIsBlocking = true;
        }
        else
        {
            bIsBlocking = false;
        }
    }
}
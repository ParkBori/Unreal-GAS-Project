// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyCombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "AbilitySystemComponent.h"
#include "Shared/Tag/SharedTag.h"
#include "Shared/Macro/Log.h"

#include "AIController.h"
#include "AI/AIBlackBoardData.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyCombatComponent::UEnemyCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;
}

void UEnemyCombatComponent::InitializeComponent()
{
    Super::InitializeComponent();

    for (auto& Pair : mAbilities)
    {
        if (Pair.Value.AbilityClass)
        {
            FGameplayAbilitySpec Spec(Pair.Value.AbilityClass, 1);
            Pair.Value.SpecHandle = mASC->GiveAbility(Spec);
        }
    }
}

void UEnemyCombatComponent::SetTarget(AActor* NewTarget)
{
    if (IsValid(NewTarget))
    {
        mCurrentTarget = NewTarget;
        mCurrentTargetASC = mCurrentTarget->GetComponentByClass<UAbilitySystemComponent>();
        mASC->AddLooseGameplayTag(State::State_HasTarget);
        ActivateFocus(true);
    }
    else
    {
        mCurrentTarget = nullptr;
        mCurrentTargetASC = nullptr;
        mASC->RemoveLooseGameplayTag(State::State_HasTarget);
        ActivateFocus(false);
    }
}

AActor* UEnemyCombatComponent::GetTarget() const
{
    return mCurrentTarget.Get();
}

bool UEnemyCombatComponent::HasTarget() const
{
    return mCurrentTarget.IsValid();
}

bool UEnemyCombatComponent::ActivateAbility(FGameplayAbilitySpecHandle SpecHandle)
{
    if (!mASC.IsValid())
        return false;

    FGameplayAbilitySpec* Spec = mASC->FindAbilitySpecFromHandle(SpecHandle);
    if (!Spec)
        return false;

    bool bSuccess = mASC->TryActivateAbility(SpecHandle);
    if (bSuccess)
    {
        if (Spec && Spec->GetPrimaryInstance())
        {
            Spec->GetPrimaryInstance()->OnGameplayAbilityEnded.RemoveAll(this);
            Spec->GetPrimaryInstance()->OnGameplayAbilityEnded.AddUObject(
                this,
                &ThisClass::Callback_OnEndedAbility
            );
        }
        
        mCurrentWaitEndAbilitySpecHandle = SpecHandle;
        return true;
    }

    return false;
}

bool UEnemyCombatComponent::ActivateAbilityManual(FGameplayAbilitySpecHandle SpecHandle)
{
    if (!mASC.IsValid())
        return false;

    FGameplayAbilitySpec* Spec = mASC->FindAbilitySpecFromHandle(SpecHandle);
    if (!Spec)
        return false;

    // 입력 눌림
    Spec->InputPressed = true;

    FTimerHandle& TimerHandle = mReleaseTimerMap.FindOrAdd(SpecHandle);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    if (Spec->IsActive())
    {
        mASC->AbilitySpecInputPressed(*Spec);
    }
    else
    {
        bool bSuccess = mASC->TryActivateAbility(SpecHandle);
        if (!bSuccess)
        {
            return false;
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, SpecHandle]()
        {
            if (!mASC.IsValid())
                return;
    
            FGameplayAbilitySpec* FoundSpec = mASC->FindAbilitySpecFromHandle(SpecHandle);
            if (!FoundSpec)
                return;
    
            FoundSpec->InputPressed = false;
            mASC->AbilitySpecInputReleased(*FoundSpec);
        },
        0.4,
        false
    );

    return true;
}


bool UEnemyCombatComponent::ActivateAttackAbility(float Dist)
{
    EEnemyCombatPattern CombatPattern;

    if (IsPlayerAttacking())
    {
        CombatPattern = EEnemyCombatPattern::AvoidAndAttack;
    }

    else if (IsPlayerDodging())
    {
        CombatPattern = EEnemyCombatPattern::Attack_Sprint;
    }

    // 블럭 브레이크 어택 
    else if (IsPlayerBlocking())
    {
        CombatPattern = EEnemyCombatPattern::Attack_Critical;
    }

    else if (Dist < mAIProfile.CloseAttackRange)
    {
        CombatPattern = EEnemyCombatPattern::Attack_Close;
    }
    else if (Dist < mAIProfile.MidAttackRange)
    {
        CombatPattern = EEnemyCombatPattern::Attack_Middle;
    }
    else
    {
        CombatPattern = EEnemyCombatPattern::Attack_Long;
    }

    if (!mAbilities.Contains(CombatPattern))
    {
        return false;
    }

    return ActivateAbility(mAbilities[CombatPattern].SpecHandle);
}

bool UEnemyCombatComponent::ActivateBlockAbility()
{
    float Rand = FMath::FRand(); // 0.0 ~ 1.0

    if (Rand < mAIProfile.BlockWeight)
    {
        if (!mAbilities.Contains(EEnemyCombatPattern::Block))
        {
            return false;
        }
        
        MY_LOG(Error, "");
        return ActivateAbilityManual(mAbilities[EEnemyCombatPattern::Block].SpecHandle);
    }

    return true;
}

bool UEnemyCombatComponent::CanAttack() const
{
    if (!mASC.IsValid())
        return false;

    if (!HasTarget())
        return false;

    if (IsGroggy())
        return false;

    float CurrentTime = GetWorld()->GetTimeSeconds();


    return true;
}

bool UEnemyCombatComponent::IsGroggy() const
{
    if (!mASC.IsValid())
        return false;

    return mASC->HasMatchingGameplayTag(State::State_Groggy);
}

float UEnemyCombatComponent::GetDistanceToTarget() const
{
    if (!HasTarget())
    {
        return MAX_FLT;
    }

    return FVector::Dist(GetOwner()->GetActorLocation(), mCurrentTarget->GetActorLocation());
}

bool UEnemyCombatComponent::IsInRange(float MinRange, float MaxRange) const
{
    float Distance = GetDistanceToTarget();
    return Distance >= MinRange && Distance <= MaxRange;
}

bool UEnemyCombatComponent::IsPlayerAttacking() const
{
    if (!mCurrentTarget.IsValid() || !mCurrentTargetASC.IsValid())
    {
        return false;
    }

    return mCurrentTargetASC->GetOwnedGameplayTags().HasTagExact(State::State_Attack_Active);
}

bool UEnemyCombatComponent::IsPlayerBlocking() const
{
    if (!mCurrentTarget.IsValid() || !mCurrentTargetASC.IsValid())
    {
        return false;
    }

    return mCurrentTargetASC->GetOwnedGameplayTags().HasTagExact(State::State_Blocking);
}

bool UEnemyCombatComponent::IsPlayerDodging() const
{
    if (!mCurrentTarget.IsValid() || !mCurrentTargetASC.IsValid())
    {
        return false;
    }

    return mCurrentTargetASC->GetOwnedGameplayTags().HasTagExact(State::State_Dodging);
}

bool UEnemyCombatComponent::IsPlayerDrinking() const
{
    if (!mCurrentTarget.IsValid() || !mCurrentTargetASC.IsValid())
    {
        return false;
    }

    return mCurrentTargetASC->GetOwnedGameplayTags().HasTagExact(State::State_DrinkingPotion);
}

void UEnemyCombatComponent::Callback_OnEndedAbility(UGameplayAbility* EndAbility)
{
    if (!EndAbility)
    {
        return;
    }

    if (mCurrentWaitEndAbilitySpecHandle == EndAbility->GetCurrentAbilitySpecHandle())
    {
        mOnAttackAbilityEnded.Broadcast();
        // 값 비워주기
        mCurrentWaitEndAbilitySpecHandle = FGameplayAbilitySpecHandle();
    }

    ActivateFocus(true);
}

void UEnemyCombatComponent::RotateToTarget()
{
    // 여기서 스페셜 어택을 하고있다면, 보간 속도가 더욱 더 빨라야함 
    if (!mCurrentTarget.IsValid())
    {
        return;
    }

    auto* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), mCurrentTarget->GetActorLocation());
    FRotator CurrentRotator = Owner->GetActorRotation();

    FRotator InterpRot = FMath::RInterpTo(CurrentRotator, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f);
    InterpRot.Pitch = CurrentRotator.Pitch;
    InterpRot.Roll = CurrentRotator.Roll;
    Owner->SetActorRotation(InterpRot);
}

AAIController* UEnemyCombatComponent::GetAIController() const
{
    return Cast<AAIController>(GetOwner()->GetInstigatorController());
}

void UEnemyCombatComponent::ActivateFocus(bool bActivate)
{
    auto AIC = GetAIController();
    if (!AIC)
    {
        return;
    }

    if (bActivate && mCurrentTarget.IsValid())
    {
        AIC->SetFocus(mCurrentTarget.Get());
    }
    else
    {
        AIC->ClearFocus(EAIFocusPriority::Gameplay);
    }
}

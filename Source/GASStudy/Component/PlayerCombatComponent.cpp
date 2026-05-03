// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Shared/Tag/SharedTag.h"

#include "Shared/Macro/Log.h"

#include "Kismet/GameplayStatics.h"



// Sets default values for this component's properties
UPlayerCombatComponent::UPlayerCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;
	// ...
}

// Called when the game starts
void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UPlayerCombatComponent::AddCombatInput(int32 InputID)
{
    if (!mASC.IsValid())
    {
        return;
    }

    // 현재 입력에 대응되는 어빌리티가 실행이 되지 않은 상태이다.
    if (FGameplayAbilitySpec* Spec = mASC->FindAbilitySpecFromInputID(InputID))
    {
        if (!Spec->IsActive())
        {
            MY_LOG(Warning, "");
            Spec->InputPressed = true;
            mASC->TryActivateAbility(Spec->Handle);
            return;
        }
    }

    // 실패 시 단일 버퍼에 저장 (덮어쓰기)
    mPendingInputID = InputID;
    HandleInputTimer();
}

void UPlayerCombatComponent::ActivateCombatCancle(bool bActivate)
{
    bCanCombatCancle = bActivate;


    MY_LOG(Warning, "");

    if (bCanCombatCancle)
    {
        // 점점 시간에 다가왔다가, 다시 빨라지는 느낌으로  --> 이런 모양의 그래프로 슬로우모션 진행 \/
        SetWorldTimeDilation(0.5f);
    }
    else
    {
        SetWorldTimeDilation(1.f);
    }
}

void UPlayerCombatComponent::ConsumeInput()
{
    if (!mPendingInputID.IsSet() || !mASC.IsValid())
    {
        MY_LOG(Warning, "ConsumeInput 입력 x ");
        return;
    }


    MY_LOG(Warning, "ConsumeInput 입력 o ");


    int32 InputID = mPendingInputID.GetValue();

    ActivateAbilityByInputID(InputID);
    mPendingInputID.Reset();
}

bool UPlayerCombatComponent::ActivateAbilityByInputID(int32 InputID)
{
    FGameplayAbilitySpec* Spec = mASC->FindAbilitySpecFromInputID(InputID);
    if (Spec)
    {
        Spec->InputPressed = true;
        if (Spec->IsActive())
        {
            mASC->AbilitySpecInputPressed(*Spec);
        }
        else
        {
            mASC->TryActivateAbility(Spec->Handle);
        }

        return true;
    }

    return false;
}

void UPlayerCombatComponent::SetWorldTimeDilation(float Value)
{
    if (UWorld* World = GetWorld())
    {
        MY_LOG(Warning, "%f", Value);
        World->GetWorldSettings()->SetTimeDilation(Value);
    }
}

void UPlayerCombatComponent::HandleInputTimer()
{
    UWorld* World = GetWorld();
    World->GetTimerManager().ClearTimer(mInputTimerHandle);

    if (!mPendingInputID.IsSet())
    {
        return;
    }
    // 기존 타이머 제거 

    World->GetTimerManager().SetTimer(mInputTimerHandle, [this]() {
        MY_LOG(Warning, "인풋버버 비웁니다.");
        mPendingInputID.Reset();
        }, InputBufferTime, false, InputBufferTime);
}






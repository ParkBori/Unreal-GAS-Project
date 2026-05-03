// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_Finisher.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "MotionWarpingComponent.h"
#include "Shared/Tag/SharedTag.h"
#include "Components/CapsuleComponent.h"

#include "Character/Playable/PlayableCharacter.h"

#include "GameplayAbilitySystem/GameplayEffect/GE_Damage.h"

#include "Subsystem/PopupManageSubsystem.h"

UGA_Finisher::UGA_Finisher()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // 기본태그
    FGameplayTagContainer AbilityTag;
    AbilityTag.AddTag(Ability::Ability_Combat_Finisher);
    AbilityTag.AddTag(Ability::Ability_Combat);
    SetAssetTags(AbilityTag);

    // 활성화 소유 태그 
    ActivationOwnedTags.AddTag(State::State_ActivateFinisher);

    // 이벤트 트리거 태그
    FAbilityTriggerData TriggerData;
    TriggerData.TriggerTag = Event::Event_Ability_Finisher;
    AbilityTriggers.Add(TriggerData);
}

void UGA_Finisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    bool bIsBack = TriggerEventData->EventMagnitude == 1.f;
    const AActor* Target = TriggerEventData->Target;
    if (!Target)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    APlayableCharacter* Player = Cast<APlayableCharacter>(GetAvatarActorFromActorInfo());

    FVector TargetLocation;
    FRotator TargetRotation;
    float DistanceOffset = 0.0f;
    FGameplayEventData EventData;

    if (bIsBack)
    {
        // [뒤에서 암살]
        mPlayMontage = mBack;

        EventData.EventMagnitude = 1.f;

        // 적의 뒤쪽으로 오프셋만큼 이동 (Forward의 반대 방향)
        DistanceOffset = mBackLocationOffset.X; // 오프셋 거리값 (예: 100.0f)
        TargetLocation = Target->GetActorLocation() - (Target->GetActorForwardVector() * DistanceOffset);

        // 회전: 적이 바라보는 방향과 똑같이 바라봄 (적의 뒤통수를 보며 찌르기)
        TargetRotation = Target->GetActorRotation();
        Player->SwitchCamer(ECameraType::FinisherBack);
    }
    else
    {
        // [앞에서 처형]
        mPlayMontage = mFront;
        // 적의 앞쪽으로 오프셋만큼 이동 (Forward 방향)
        DistanceOffset = mFrontLocationOffset.X; // 오프셋 거리값 (예: 100.0f)
        TargetLocation = Target->GetActorLocation() + (Target->GetActorForwardVector() * DistanceOffset);

        // 회전: 적을 정확히 마주봄
        TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, Target->GetActorLocation());
        Player->SwitchCamer(ECameraType::FinisherFront);
    }

    // Pitch/Roll 값에 의해 캐릭터가 기울어지지 않도록 Yaw만 추출
    TargetRotation = FRotator(0.f, TargetRotation.Yaw, 0.f);

    // Motion Warping 등록
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (UMotionWarpingComponent* MotionWarpingComp = Avatar->FindComponentByClass<UMotionWarpingComponent>())
    {
        MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("Finisher"), TargetLocation, TargetRotation);
    }


    if (bFirstActive)
    {
        bFirstActive = false;
        auto* SS = GetWorld()->GetGameInstance()->GetSubsystem<UPopupManageSubsystem>();
        if (SS)
        {
            SS->CreatePopupFactory(EPopupType::Tutorial_Finisher);
        }
    }


	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(Target), Event::Event_Ability_Finished, EventData);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Finisher::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    APlayableCharacter* Player = Cast<APlayableCharacter>(GetAvatarActorFromActorInfo());
    Player->SwitchCamer(ECameraType::Main);

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


bool UGA_Finisher::CheckPlayerIsTargetBack()
{
	const AActor* SourceActor = GetAvatarActorFromActorInfo();
	const AActor* TargetActor = CurrentEventData.Target;

	// 앞뒤판별 
	FVector Enemy2Target = SourceActor->GetActorLocation() - TargetActor->GetActorLocation();
	Enemy2Target.Normalize();
	float DotValue = FVector::DotProduct(TargetActor->GetActorForwardVector(), Enemy2Target);
	
	IsPlayerBack = DotValue > 0.7f ? false : true;

	return IsPlayerBack;
}

UGA_Finished::UGA_Finished()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // 기본태그
    FGameplayTagContainer AbilityTag;
    AbilityTag.AddTag(Ability::Ability_Combat_Finished);
    SetAssetTags(AbilityTag);

    // 활성화 소유 태그 
    ActivationOwnedTags.AddTag(State::State_ActivateFinished);

    // 이벤트 트리거 태그
    FAbilityTriggerData TriggerData;
    TriggerData.TriggerTag = Event::Event_Ability_Finished;
    AbilityTriggers.Add(TriggerData);
}

void UGA_Finished::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TriggerEventData->EventMagnitude == 1.f ? (mPlayMontage = mBack) : (mPlayMontage = mFront);

    auto* CC = GetAvatarActorFromActorInfo()->FindComponentByClass<UCapsuleComponent>();
    CC->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        UGE_NormalDamage::StaticClass(), 1.f, TriggerEventData->ContextHandle);

    SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param_DmgCoefficient, 999.f);
    SpecHandle.Data->SetSetByCallerMagnitude(Effect::Effect_Param_Knockback, 0.f);
    ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);


    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

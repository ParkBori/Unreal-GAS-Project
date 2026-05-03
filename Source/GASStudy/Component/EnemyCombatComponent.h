// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Component/CombatComponent.h"
#include "Shared/Commbat/CombatData.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "EnemyCombatComponent.generated.h"


class UAbilitySystemComponent;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FEnemyAIProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    float BlockWeight = 0.85f;

    UPROPERTY(EditAnywhere)
    float AvoidWeight = 0.7f;

    UPROPERTY(EditAnywhere)
    float LongAttackRange   = 700;
    
    UPROPERTY(EditAnywhere)
    float MidAttackRange    = 400;
   
    UPROPERTY(EditAnywhere)
    float CloseAttackRange  = 200;

};

UENUM(BlueprintType)
enum class EEnemyCombatPattern : uint8
{
    Attack_Long,        // 먼거 리에 있을때 사용하는 공격
    Attack_Middle,      // 중간 거리에 있을때 사용하는 공격
    Attack_Close,       // 근접 거리에 있을때 사용하는 공격 
    Attack_Sprint,
    Attack_Critical,  // 가드 파괴 공격 
    AvoidAndAttack,     // 닷지 + 공격 
    Block,

    None,
};

USTRUCT(BlueprintType)
struct FEnemyAbilityData
{
    GENERATED_BODY()

    // 에디터 세팅
    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameplayAbility> AbilityClass = nullptr;
    
    // 런타임
    UPROPERTY()
    FGameplayAbilitySpecHandle SpecHandle;
};


DECLARE_MULTICAST_DELEGATE(FOnAbilityEnded);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GASSTUDY_API UEnemyCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

public:
    UEnemyCombatComponent();

protected:
    virtual void InitializeComponent() override;

public:
    /* Target */
    void SetTarget(AActor* NewTarget);
    AActor* GetTarget() const;
    bool HasTarget() const;

    /** 
    * 어빌리티가 유지되고 알아서 종료되는 어빌리티 
    * ex) 공격 어빌리티
    */
    bool ActivateAbility(FGameplayAbilitySpecHandle SpecHandle);

    /**
    * 어빌리티 종료시점을 직접 세팅하는 어빌리티
    * ex) 블럭
    */
    bool ActivateAbilityManual(FGameplayAbilitySpecHandle SpecHandle);

    /* 거리기반 공격 어빌리티 */
    bool ActivateAttackAbility(float Dist);

    /* 블럭 어빌리티 */
    bool ActivateBlockAbility();

    /* Combat State */
    bool CanAttack() const;

    /* Groggy */
    bool IsGroggy() const;

    /* Range */
    float GetDistanceToTarget() const;
    bool IsInRange(float MinRange, float MaxRange) const;

    void RotateToTarget();

    class AAIController* GetAIController() const;

    void ActivateFocus(bool bActivate);

public:
    /* TargetActor State */

    /* 플레이어 공격 상태 */
    bool IsPlayerAttacking() const; 
    
    /* 플레이어 가드 상태*/
    bool IsPlayerBlocking() const;

    /* 플레이어 닷지 or 롤링 상태 */
    bool IsPlayerDodging() const;

    /* 플레이어 포션 마시는 상태 */
    bool IsPlayerDrinking() const;


private:
    UFUNCTION()
    void Callback_OnEndedAbility(UGameplayAbility* EndAbility);

protected:
    UPROPERTY(EditAnywhere, Category = "Combat|Profile")
    FEnemyAIProfile mAIProfile;
    
    //UPROPERTY(EditAnywhere, Category = "Combat|Ability")
    //TMap<EEnemyCombatPattern, TSubclassOf<UGameplayAbility>> mAbilities;
    //TMap<EEnemyCombatPattern, FGameplayAbilitySpecHandle> mAbilitySpecHandle;

    UPROPERTY(EditAnywhere, Category = "Combat|Ability")
    TMap<EEnemyCombatPattern, FEnemyAbilityData> mAbilities;

    UPROPERTY()
    TWeakObjectPtr<AActor> mCurrentTarget;

    UPROPERTY()
    TWeakObjectPtr<UAbilitySystemComponent> mCurrentTargetASC;

    //EnemyCombatPattern mPreveCombatPattern = EEnemyCombatPattern::None;
    //EnemyCombatPattern mCurrentCombatPattern = EEnemyCombatPattern::None;

private:
    UPROPERTY()
    TMap<FGameplayAbilitySpecHandle, FTimerHandle> mReleaseTimerMap;

    // 현재 종료가 되길 기다리는 어빌리티 스펙핸들
    FGameplayAbilitySpecHandle mCurrentWaitEndAbilitySpecHandle;

public:
    FOnAbilityEnded mOnAttackAbilityEnded;
};

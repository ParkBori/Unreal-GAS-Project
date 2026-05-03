// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/GameplayAbility/GA_Base.h"
#include "GA_EnemyAttack.generated.h"

/**
* 적 특수 기믹이 존재하지 않고, 몽타주 노티파이로 타격판정하는 공격의 부모 클래스
* 
* BP 상속후 mPlayMontages에 재생 몽타주 기입 / 몽타주느에는 다음 몽타주로 넘어갈 수 있는 노티파이를 걸어야한다.
*/
UCLASS()
class GASSTUDY_API UGA_EnemyAttack : public UGA_Combat	
{
	GENERATED_BODY()
	
public:
	UGA_EnemyAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void Callback_AddedPlayNextMontaGeTag();

protected:
	int32 mCurrentPlayMontageIdx = 0;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "PlayMontages"))
	TArray<TObjectPtr<UAnimMontage>> mPlayMontages;
};

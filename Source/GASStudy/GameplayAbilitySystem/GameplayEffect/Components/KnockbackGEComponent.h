// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffect.h"
#include "KnockbackGEComponent.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EKnockbackGEMagnitudeCalculation : uint8
{
	ScalableFloat,
	SetByCaller
};

// [Struct] FKnockbackGEMagnitude
// Desc: GEMagnitude에서 설정할 수 있는 Float 객체
// Usage: 
USTRUCT(BlueprintType)
struct FKnockbackGEMagnitude
{
	GENERATED_BODY()

public:
	float GetMagnitueValue(const FGameplayEffectSpec& EffectSpec, float DefaultVal = 0.f) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Knockback Magnitude", meta = (DisplayName = "MagnitudeType"))
	EKnockbackGEMagnitudeCalculation mMagnitudeType = EKnockbackGEMagnitudeCalculation::ScalableFloat;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback Magnitude", meta = (DisplayName = "ScalableMagnitude", EditCondition = "mMagnitudeType == EConditionalGEMagnitudeCalculation::ScalableFloat", EditConditionHides))
	FScalableFloat mScalableMagnitude;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback Magnitude", meta = (DisplayName = "SetByCallerMagnitude", EditCondition = "mMagnitudeType == EConditionalGEMagnitudeCalculation::SetByCaller", EditConditionHides))
	FSetByCallerFloat mSetByCallerMagnitude;
};

UCLASS()
class GASSTUDY_API UKnockbackGEComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UKnockbackGEComponent();

public:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const;

public:
	void SetKnockbackPower(float Power);
	void SetKnockbackPower(FGameplayTag SetByCallerTag);
	void SetIgnoreKnockbackZHit(bool bIgnoreZHit, float DefaultZKnockback = 0.f);

protected:
	UPROPERTY(Category = Knockback, EditDefaultsOnly, meta = (DisplayName = "KnockbackPower"))
	FKnockbackGEMagnitude mKnockbackPower;

	UPROPERTY(Category = Knockback, EditDefaultsOnly, meta = (DisplayName = "IgnoreZHit"))
	bool mIgnoreZHit;

	UPROPERTY(Category = Knockback, EditDefaultsOnly, meta = (DisplayName = "DefaultZKnockback", EditCondition = "mIgnoreZHit == true", EditConditionHides))
	float mDefaultZKnockback;
};

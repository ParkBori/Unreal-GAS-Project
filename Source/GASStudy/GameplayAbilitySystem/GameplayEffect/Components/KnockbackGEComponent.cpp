// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/Components/KnockbackGEComponent.h"
#include "GameFramework/Character.h"

float FKnockbackGEMagnitude::GetMagnitueValue(const FGameplayEffectSpec& EffectSpec, float DefaultVal) const
{
	switch (mMagnitudeType)
	{
	case EKnockbackGEMagnitudeCalculation::ScalableFloat:
		return mScalableMagnitude.GetValueAtLevel(EffectSpec.GetLevel());
	case EKnockbackGEMagnitudeCalculation::SetByCaller:
		return EffectSpec.GetSetByCallerMagnitude(mSetByCallerMagnitude.DataTag, false, DefaultVal);
	default:
		return DefaultVal;
	}
}

UKnockbackGEComponent::UKnockbackGEComponent()
{
#if WITH_EDITORONLY_DATA
	EditorFriendlyName = TEXT("Knockback");
#endif
}

void UKnockbackGEComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	const FHitResult* HitRes = GESpec.GetContext().GetHitResult();
	if (HitRes != nullptr)
	{
		auto* TargetChar = Cast<ACharacter>(HitRes->GetActor());
		if (TargetChar != nullptr)
		{
			FVector KnockBackVec;
			if (mIgnoreZHit == true)
			{
				KnockBackVec = -HitRes->ImpactNormal.GetSafeNormal2D() * mKnockbackPower.GetMagnitueValue(GESpec);
				KnockBackVec.Z = mDefaultZKnockback;
			}
			else
			{
				KnockBackVec = -HitRes->ImpactNormal * mKnockbackPower.GetMagnitueValue(GESpec);
			}
			TargetChar->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
			TargetChar->LaunchCharacter(KnockBackVec, true, true);
		}
	}
}

void UKnockbackGEComponent::SetKnockbackPower(float Power)
{
	mKnockbackPower.mMagnitudeType = EKnockbackGEMagnitudeCalculation::ScalableFloat;
	mKnockbackPower.mScalableMagnitude.SetValue(Power);
}

void UKnockbackGEComponent::SetKnockbackPower(FGameplayTag SetByCallerTag)
{
	mKnockbackPower.mMagnitudeType = EKnockbackGEMagnitudeCalculation::SetByCaller;
	mKnockbackPower.mSetByCallerMagnitude.DataTag = SetByCallerTag;
}

void UKnockbackGEComponent::SetIgnoreKnockbackZHit(bool bIgnoreZHit, float DefaultZKnockback)
{
	mIgnoreZHit = bIgnoreZHit;
	if (mIgnoreZHit == true)
	{
		mDefaultZKnockback = DefaultZKnockback;
	}
}

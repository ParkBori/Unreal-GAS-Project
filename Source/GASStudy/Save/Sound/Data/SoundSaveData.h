// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundSaveData.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FSoundSaveData
{
	GENERATED_BODY()

	FSoundSaveData() {}

	const FSoundSaveData& operator = (float Value)
	{
		/*Master = */BGM = SFX = UI = Value;
		return *this;
	}

	bool operator == (const FSoundSaveData& Other) const
	{
		return /*FMath::IsNearlyEqual(Other.Master, Master)
			&& */FMath::IsNearlyEqual(Other.BGM, BGM)
			&& FMath::IsNearlyEqual(Other.SFX, SFX)
			&& FMath::IsNearlyEqual(Other.UI, UI);
	}

	//UPROPERTY()
	//float Master = 0.f;

	UPROPERTY()
	float BGM = 0.f;

	UPROPERTY()
	float SFX = 0.f;

	UPROPERTY()
	float UI = 0.f;
};
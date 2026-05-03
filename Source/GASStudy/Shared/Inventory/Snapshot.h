// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Snapshot.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInventoryUISnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bEmpty = false;

	UPROPERTY(BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentAmount = 0;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;

	FString ToString() const
	{
		return FString::Printf(TEXT("%d"), CurrentAmount);
	}
};

USTRUCT(BlueprintType)
struct FStatUISnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool  bUsePreview = false;

	UPROPERTY(BlueprintReadOnly)
	float CurrentValue = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float PreviewValue = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float DiffValue = 0.f;

	FString ToString() const
	{
		FString UsePreview = (bUsePreview == true) ? TEXT("true") : TEXT("false");
		return FString::Printf(TEXT("UsePreview: %s CurrentValue: %f, PreviewValue: %f, DiffValue: %f,"), *UsePreview, CurrentValue, PreviewValue, DiffValue);
	}
};

using FStatSnapshotMap = TMap<FGameplayAttribute, FStatUISnapshot>;

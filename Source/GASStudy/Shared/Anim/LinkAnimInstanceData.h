// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LinkAnimInstanceData.generated.h"

/**
 * 무기 태그 - 애니메이션 
 */
USTRUCT(BlueprintType)
struct FLinkAnimInstanceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim", meta = (DisplayName = "LinkAnimInstance"))
	TSubclassOf<UAnimInstance> LinkAnimInstance = nullptr;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 인터렉트 오브젝트들의 커스텀 뎁스값을 정의
 */
UENUM(BlueprintType)
enum class EInteractCustomStencil : uint8
{
    None = 0 UMETA(DisplayName = "None"),
    Item = 1 UMETA(DisplayName = "Item"),
    Door = 2 UMETA(DisplayName = "Door"),
    Quest = 3 UMETA(DisplayName = "Quest"),
    Enemy = 4 UMETA(DisplayName = "Enemy")
};
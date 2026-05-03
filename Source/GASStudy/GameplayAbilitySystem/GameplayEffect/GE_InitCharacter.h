// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_InitCharacter.generated.h"

/**
 * 플레이어 및 적 GAS 어빌리티 초기 세팅용 이펙트
 * 
 * 
 */
UCLASS()
class GASSTUDY_API UGE_InitCharacter : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UGE_InitCharacter();
	
};

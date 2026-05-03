// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Damage.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGE_Damage : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UGE_Damage();
};

/**
* 일반 데미지
*/
UCLASS()
class GASSTUDY_API UGE_NormalDamage : public UGE_Damage
{
	GENERATED_BODY()

public:
	UGE_NormalDamage();
};

/**
* 고정 데미지
*/
UCLASS()
class GASSTUDY_API UGE_TrueDamage : public UGE_Damage
{
	GENERATED_BODY()

public:
	UGE_TrueDamage();
};

/**
* 광역 데미지 
*/
UCLASS()
class GASSTUDY_API UGE_RadiusDamage : public UGE_Damage
{
	GENERATED_BODY()

public:
	UGE_RadiusDamage();
};
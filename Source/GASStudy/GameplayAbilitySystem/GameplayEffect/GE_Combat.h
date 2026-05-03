// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Combat.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGE_Combat : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UGE_Combat();
};

/**
* 플레이어 스테미나
*/
UCLASS()
class GASSTUDY_API UGE_Stamina : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Stamina();
};

/**
* 블럭시 디메리트
*/
UCLASS()
class GASSTUDY_API UGE_Groggy: public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Groggy();
};


UCLASS()
class GASSTUDY_API UGE_Regen : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Regen();

protected:
	void AddRegenModifier(FGameplayAttribute Attribute, float Amount);



};

UCLASS()
class GASSTUDY_API UGE_StaminaRegen : public UGE_Regen
{
	GENERATED_BODY()

public:
	UGE_StaminaRegen();
};

UCLASS()
class GASSTUDY_API UGE_GroggyRegen : public UGE_Regen
{
	GENERATED_BODY()

public:
	UGE_GroggyRegen();
};
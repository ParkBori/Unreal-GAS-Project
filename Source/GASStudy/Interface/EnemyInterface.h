// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterInfo.h"
#include "UObject/Interface.h"
#include "AI/Data/AIAttackData.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSTUDY_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//virtual void GetAttackDatTable
	virtual bool DoBlock() = 0;
	virtual void EndBlock() = 0;
	virtual void AddDamagedCount() = 0;
	virtual int32 GetDamagedCount() = 0;
	virtual void RestDamagedCount() = 0;
	virtual bool CheckCanCounter() = 0;
	virtual bool DoCounter(ECounterType CounterType) = 0;
	virtual void EndCounter() = 0;
	virtual bool DoDodge() = 0;
	virtual bool CanFinishCounter() = 0;
	virtual bool DoSpecialAttack() = 0;
	
	
	UFUNCTION(BlueprintCallable)
	virtual void SetFocus(bool NewFocus)  =  0;
	
	virtual void SetEnemyGait(EGait NewGait) = 0;
};

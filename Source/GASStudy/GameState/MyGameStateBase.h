// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnGameClear);

UCLASS()
class GASSTUDY_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	FOnGameClear OnGameClear;
};

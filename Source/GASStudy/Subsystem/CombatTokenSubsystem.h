// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatTokenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UCombatTokenSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
    bool TryAcquireToken()
    {
        if (CurrentToken >= MaxToken)
            return false;

        CurrentToken++;
        return true;
    }

    void ReleaseToken()
    {
        CurrentToken = FMath::Max(0, CurrentToken - 1);
    }
	

private:
    int32 MaxToken = 2;

    int32 CurrentToken = 0;

	
};

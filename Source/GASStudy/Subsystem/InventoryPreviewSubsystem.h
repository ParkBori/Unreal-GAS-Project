// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InventoryPreviewSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UInventoryPreviewSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
    void SetPreviewCharacter(AActor* InActor)
    {
        PreviewCharacter = InActor;
    }

    AActor* GetPreviewCharacter() const
    {
        return PreviewCharacter.Get();
    }

private:
    TWeakObjectPtr<AActor> PreviewCharacter;
};

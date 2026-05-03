// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ContentPanenlUIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UContentPanenlUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSTUDY_API IContentPanenlUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Back() = 0;
	virtual void ApplyChange() = 0;
	virtual void Reset() = 0;
};

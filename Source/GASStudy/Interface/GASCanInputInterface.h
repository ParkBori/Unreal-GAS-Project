// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GASCanInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGASCanInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSTUDY_API IGASCanInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetCanInput(bool InCanInput) = 0;
	virtual void SetReStart(bool InReStart) { ReStart = InReStart; }
	
	bool GetCanInput() { return CanInput; }
	bool GetReStart() { return ReStart; }

protected:
	bool ReStart = false;
	bool CanInput = false;
};

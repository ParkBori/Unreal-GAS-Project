// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GASStudyGameMode.generated.h"

UCLASS(minimalapi)
class AGASStudyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGASStudyGameMode();

protected:
	virtual void BeginPlay() override;
	
protected:
};




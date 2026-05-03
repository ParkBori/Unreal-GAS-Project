// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIFccusPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API AUIFccusPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AUIFccusPlayerController();


protected:
	virtual void OnPossess(APawn* aPawn) override;

private:
	
	
};

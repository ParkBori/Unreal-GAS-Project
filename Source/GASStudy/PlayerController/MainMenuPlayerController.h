// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UMainMenuPanelWidget;
/**
 * 
 */
UCLASS()
class GASSTUDY_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuPlayerController();

protected:
	virtual void OnPossess(APawn* aPawn) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuPanelWidget> mMainMenuClass;

	UPROPERTY()
	TObjectPtr<UMainMenuPanelWidget> mMainMenuInstance;
};

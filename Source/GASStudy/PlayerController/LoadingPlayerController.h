// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoadingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ALoadingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALoadingPlayerController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Loading|UI")
	TSubclassOf<UUserWidget>	mLoadingWidgetClass;

	UPROPERTY()
	TObjectPtr<class ULoadingWidget> mLoadingWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LoadingPlayerController.h"
#include "UI/Loading/LoadingWidget.h"

ALoadingPlayerController::ALoadingPlayerController()
{
	bShowMouseCursor = false;
}

void ALoadingPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FInputModeGameAndUI	InputMode;
	SetInputMode(InputMode);

	if (IsValid(mLoadingWidgetClass))
	{
		mLoadingWidget = CreateWidget<ULoadingWidget>(GetWorld(), mLoadingWidgetClass);

		if (IsValid(mLoadingWidget))
		{
			mLoadingWidget->AddToViewport();
		}
	}
}

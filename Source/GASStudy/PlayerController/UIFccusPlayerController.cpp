// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/UIFccusPlayerController.h"
#include "UIFccusPlayerController.h"

AUIFccusPlayerController::AUIFccusPlayerController()
{
	bShowMouseCursor = true;
}

void AUIFccusPlayerController::OnPossess(APawn* aPawn)
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}

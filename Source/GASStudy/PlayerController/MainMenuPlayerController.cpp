// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/MainMenuPlayerController.h"
#include "UI/Pannel/MainMenuPanel/MainMenuPanelWidget.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainMenuPanelWidget> MainMenuAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Panel/MainMenuPanel/WBP_MainMenu.WBP_MainMenu_C'"));
	if (MainMenuAsset.Succeeded())
	{
		mMainMenuClass = MainMenuAsset.Class;
	}
}

void AMainMenuPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (IsValid(mMainMenuInstance))
	{
		return;
	}

	mMainMenuInstance = CreateWidget<UMainMenuPanelWidget>(this, mMainMenuClass);
	if (IsValid(mMainMenuInstance))
	{
		mMainMenuInstance->AddToViewport();
	
		
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetShowMouseCursor(true);

		SetInputMode(InputMode);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/MainMenuPanel/MainMenuPanelWidget.h"
#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"
#include "UI/Pannel/ContentPannel/SoundPannelWidget.h"
#include "UI/Pannel/ContentPannel/GraphicPanelWidget.h"
#include "UI/Pannel/BasePannel/SettingBasePannelWidget.h"
#include "Subsystem/PopupManageSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Pannel/LoadPannel/LoadWidget.h"

#include "Shared/Macro/Log.h"


#include "Subsystem/WorldTransitionSubsystem.h"
#include "Subsystem/SaveSubsystem.h"



void UMainMenuPanelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
	 // ==== Main Menu ====
	if (IsValid(Btn_Continue))
	{
		Btn_Continue->OnClicked().AddUObject(this, &UMainMenuPanelWidget::Callback_ClickedBtnContinue);
	}

	if (IsValid(Btn_NewGame))
	{
		Btn_NewGame->OnClicked().AddUObject(this, &UMainMenuPanelWidget::Callback_ClickedBtnNewGame);
	}

	if (IsValid(Btn_Load))
	{
		Btn_Load->OnClicked().AddLambda([&]() { ChangeState(EMainMenuState::LOAD); });
	}

	if (IsValid(Btn_Setting))
	{
		Btn_Setting->OnClicked().AddLambda([&]() { ChangeState(EMainMenuState::SETTINGS); });
	}

	if (IsValid(Btn_Exit))
	{
		Btn_Exit->OnClicked().AddUObject(this, &UMainMenuPanelWidget::Callback_ClickedBtnExit);
	}

	// ==== Setting ====
	if (IsValid(Btn_GraphicSetting))
	{
		Btn_GraphicSetting->OnClicked().AddLambda([&]() { CreateSettingWidget(ESettingType::GRAPHIC); });
	}

	if (IsValid(Btn_SoundSetting))
	{
		Btn_SoundSetting->OnClicked().AddLambda([&]() { CreateSettingWidget(ESettingType::SOUND); });
	}

	if (IsValid(Btn_Back))
	{
		Btn_Back->OnClicked().AddLambda([&]() { ChangeState(EMainMenuState::MAIN); });
	}

	// Load
	if (Load)
	{
		// 로드는, 버튼을 로드쪽에서 소유하기 때문
		Load->mOnClickedBackButn.AddLambda([&]() { ChangeState(EMainMenuState::MAIN); });
	}

	if (IsValid(FadeAnim))
	{
		FWidgetAnimationDynamicEvent WidgetAnimationDynamicEvent;
		WidgetAnimationDynamicEvent.BindUFunction(this, TEXT("SettingWidgetAddViewport"));
		BindToAnimationFinished(FadeAnim, WidgetAnimationDynamicEvent);
	}

	ChangeState(EMainMenuState::MAIN);
}

void UMainMenuPanelWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMainMenuPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainMenuPanelWidget::ChangeState(EMainMenuState NewState)
{
	switch (NewState)
	{
	case EMainMenuState::MAIN:
		CAS_MainMenu->SetActiveWidget(OV_MainMenu);
		break;
	case EMainMenuState::NEWGAME:
		CAS_MainMenu->SetActiveWidget(OV_MainMenu);
		break;
	case EMainMenuState::LOAD:
		CAS_MainMenu->SetActiveWidget(OV_Load);
		Load->Reset();
		break;
	case EMainMenuState::SETTINGS:
		CAS_MainMenu->SetActiveWidget(OV_Setting);
		break;
	}
}

void UMainMenuPanelWidget::CreateSettingWidget(ESettingType NewSettingType)
{
	// 애니메이션 재생이 완전히 종료되면 띄어준다
	TSubclassOf<USettingContentPannelWidget> PannelClass;

	switch (NewSettingType)
	{
	case ESettingType::GRAPHIC:
		PannelClass = mGraphiPanelClass;
		break;
	case ESettingType::SOUND:
		PannelClass = mSoundPanelClass;
		break;
	}

	APlayerController* PC = GetOwningPlayer<APlayerController>();
	if (IsValid(PC))
	{
		mCurrentActivSettingWidget = CreateWidget<USettingBasePannelWidget>(PC, mSettingBasePanelClass);
		mCurrentActivSettingWidget->mOnDestructWidget.AddLambda([&]() {
			mCurrentActivSettingWidget = nullptr;
			PlayAnimation(FadeAnim, 0.f);
			});
		mCurrentActivSettingWidget->InitWidget(PannelClass);
		if (IsValid(mCurrentActivSettingWidget) && IsValid(FadeAnim))
		{
			PlayAnimation(FadeAnim, 0.f, 1, EUMGSequencePlayMode::Type::Reverse);
		}
	}
}

void UMainMenuPanelWidget::Callback_ClickedBtnContinue()
{
	// 새로운 슬롯을 만들주고 
	auto* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}

	SS->SelectLastSavedGame();

	auto* WTS = GetGameInstance()->GetSubsystem<UWorldTransitionSubsystem>();
	if (!WTS)
	{
		return;
	}

	FTransitionRequest Request;
	Request.LevelName = TEXT("Lvl_Temple");
	Request.IsNewGame = false;
	Request.WorldTransitionType = EWorldTransitionType::OpenLevel;

	WTS->RequestTransition(Request);

	// 가장 최근 슬롯 정보를 찾아서
}

void UMainMenuPanelWidget::Callback_ClickedBtnNewGame()
{
	// 새로운 슬롯을 만들주고 
	auto* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}

	SS->SelectNewGame();

	auto* WTS = GetGameInstance()->GetSubsystem<UWorldTransitionSubsystem>();
	if (!WTS)
	{
		return;
	}

	FTransitionRequest Request;
	Request.LevelName = TEXT("Lvl_Temple");
	Request.IsNewGame = true;
	Request.WorldTransitionType = EWorldTransitionType::OpenLevel;

	WTS->RequestTransition(Request);
}

void UMainMenuPanelWidget::SettingWidgetAddViewport()
{
	if (IsValid(mCurrentActivSettingWidget))
	{
		mCurrentActivSettingWidget->AddToViewport(2);
	}
}

void UMainMenuPanelWidget::Callback_ClickedBtnExit()
{
	UPopupManageSubsystem* PopupManageSubsystem = GetGameInstance()->GetSubsystem<UPopupManageSubsystem>();
	if (IsValid(PopupManageSubsystem))
	{
		FPopupInfo PopupInfo;
		PopupInfo.bOnlyConfirm = false;
		PopupInfo.Title = FText::FromString(TEXT("Exit"));
		PopupInfo.Script = FText::FromString(TEXT("Are you sure you want to quit th game?"));
		PopupInfo.OnConfirmDelegate.AddDynamic(this, &UMainMenuPanelWidget::Callback_ConfirmExitPopup);
		// PopupInfo.OnCancleDelegate.AddLambda([]() {});
		PopupManageSubsystem->CreatePopup(PopupInfo);
	}
}

void UMainMenuPanelWidget::Callback_ConfirmExitPopup()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

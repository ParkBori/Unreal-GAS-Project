// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/UIInfo.h"
#include "MainMenuPanelWidget.generated.h"


enum class EMainMenuState : uint8
{
	MAIN,
	NEWGAME,
	LOAD,
	SETTINGS,
};

enum class ESettingType : uint8
{
	GRAPHIC,
	SOUND,
};


class UBaseButtonWidget;

/**
 * 시작화면 UI 
 */
UCLASS()
class GASSTUDY_API UMainMenuPanelWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> CAS_MainMenu;

private:
	void ChangeState(EMainMenuState NewState);
	void CreateSettingWidget(ESettingType NewSettingType);

	UFUNCTION()
	void Callback_ClickedBtnContinue();

	UFUNCTION()
	void Callback_ClickedBtnNewGame();

	UFUNCTION()
	void SettingWidgetAddViewport();

	void Callback_ClickedBtnExit();

	UFUNCTION()
	void Callback_ConfirmExitPopup();
	
#pragma region MAINMENU
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OV_MainMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Continue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_NewGame;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Load;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Setting;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Exit;
#pragma endregion 

#pragma region SETTINGS
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OV_Setting;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_GraphicSetting;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_SoundSetting;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBaseButtonWidget> Btn_Back;
	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGraphicPanelWidget> mGraphiPanelClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USoundPannelWidget> mSoundPanelClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USettingBasePannelWidget> mSettingBasePanelClass;
	
	//UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USettingBasePannelWidget> mCurrentActivSettingWidget;
#pragma endregion 

#pragma region LOAD
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OV_Load;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULoadWidget> Load;
#pragma endregion 

private:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeInOut_CreateSetting;
};

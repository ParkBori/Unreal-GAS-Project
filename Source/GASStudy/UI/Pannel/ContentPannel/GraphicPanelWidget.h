// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Pannel/ContentPannel/SettingContentPannelWidget.h"
#include "GraphicPanelWidget.generated.h"


class UOptionCycleButtonWidget;
class UOptionSliderWidget;

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGraphicPanelWidget 
	: public USettingContentPannelWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	virtual bool CheckIsAnySettingDirty() override;

	virtual void Back() override;
	virtual void ApplyChange() override;
	virtual void Reset() override;

	virtual void Confirm() override;
	virtual void Revert() override;

protected:
	void BindOnHoverded_Button(TArray<TObjectPtr<UOptionCycleButtonWidget>>& ButtonArr);
	void BindOnUnHoverded_Button(TArray<TObjectPtr<UOptionCycleButtonWidget>>& ButtonArr);

	void BindOnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr);
	void BindOnUnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr);

	void OnHoveredCallback_Button(UOptionCycleButtonWidget* Button);
	void OnUnHoveredCallback_Button(UOptionCycleButtonWidget* Button);

	void OnHoveredCallback_Slider(UOptionSliderWidget* Slider);
	void OnUnHoveredCallback_Slider(UOptionSliderWidget* Slider);

private:
	void UpdateButtons();
	void BindUserSettingFunc();
	/**
	* @param Widget : 툴팁 위치를 정해주는 위젯
	* @ 스크롤 위젯 내부에 있는 버튼의 위치를 기반으로 툴팁의 위치를 정함
	* 
	* @ 해당 기능이 슬라이더 / 버튼 각각 하고있는데, 슬라이더와 버튼의 부모를 안묶어줘서
	* 동일한 기능이 2번 작성되어 있기때문에 추후 해당함수로 교체
	*/
	void ToggleTooltip(UWidget* Widget, bool bEnable);


	UFUNCTION()
	void Callback_ConfirmBack();

	UFUNCTION()
	void Callback_CancleBack();

	
	UFUNCTION()
	void Callback_ConfirmApplyChange();

	UFUNCTION()
	void Callback_CancleApplyChange();

private:
	UFUNCTION()
	void HandleScreenResolution(int32 StateValue);
	UFUNCTION()
	void HandleWindowMode(int32 StateValue);
	UFUNCTION()
	void HandleResolutionScaleValue(float StateValue);
	UFUNCTION()
	void HandleDynamicResolution(int32 StateValue);
	UFUNCTION()
	void HandleVSync(int32 StateValue);
	UFUNCTION()	
	void HandleFrameRateLimit(int32 StateValue);
	UFUNCTION()
	void HandleOverallScalabilityLevel(int32 StateValue);
	UFUNCTION()
	void HandleAntiAliasingQuality(int32 StateValue);
	UFUNCTION()
	void HandleFoliageQuality(int32 StateValue);
	UFUNCTION()
	void HandleTextureQuality(int32 StateValue);
	UFUNCTION()
	void HandleShadingQuality(int32 StateValue);
	UFUNCTION()
	void HandleShadowQuality(int32 StateValue);
	UFUNCTION()
	void HandleReflectionQuality(int32 StateValue);
	UFUNCTION()
	void HandlePostProcessQuality(int32 StateValue);
	UFUNCTION()
	void HandleVisualEffectQuality(int32 StateValue);
	UFUNCTION()
	void HandleViewDistanceQuality(int32 StateValue);
	UFUNCTION()
	void HandleGlobalIlluminationQuality(int32 StateValue);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SCB_OptionButtons;

	// General
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_ScreenResolution;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_WindowMode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionSliderWidget> Sl_ResolutionScale;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_DynamicResolution;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_VerticalSync;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_FrameRateLimit;

	// Graphhic

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_OverallQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_AntiAliasingQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_FoliageQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_TextureQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_ShadingQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_ShadowQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_ReflectionsQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_PostProcessQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_VisualEffectQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_ViewDistanceQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOptionCycleButtonWidget> Btn_GlobalIlluminationQuality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDefaultToolTipWidget> Tooltip_OptionDesc;


private:
	int32 CurrentScreenResolution;
	int32 CurrentWindowMode;
	float CurrentResolutionScale;
	int32 CurrentDynamicResolution;
	int32 CurrentVerticalSync;
	int32 CurrentFrameRateLimit;
	int32 CurrentOverallQuality;
	int32 CurrentAntiAliasingQuality;
	int32 CurrentFoliageQuality;
	int32 CurrentTextureQuality;
	int32 CurrentShadingQuality;
	int32 CurrentShadowQuality;
	int32 CurrentReflectionsQuality;
	int32 CurrentPostProcessQuality;
	int32 CurrentVisualEffectQuality;
	int32 CurrentViewDistanceQuality;
	int32 CurrentGlobalIlluminationQuality;

	int32 NewScreenResolution;
	int32 NewWindowMode;
	float NewResolutionScale;
	int32 NewDynamicResolution;
	int32 NewVerticalSync;
	int32 NewFrameRateLimit;
	int32 NewOverallQuality;
	int32 NewAntiAliasingQuality;
	int32 NewFoliageQuality;
	int32 NewTextureQuality;
	int32 NewShadingQuality;
	int32 NewShadowQuality;
	int32 NewReflectionQuality;
	int32 NewPostProcessQuality;
	int32 NewVisualEffectQuality;
	int32 NewViewDistanceQuality;
	int32 NewGlobalIlluminationQuality;

	bool bIsChangedResolution;
	bool bIsChangedScreenMode;

private:
	UPROPERTY()
	TObjectPtr<class UUserSettingSubsystem> mUserSettingSubsystem;
};

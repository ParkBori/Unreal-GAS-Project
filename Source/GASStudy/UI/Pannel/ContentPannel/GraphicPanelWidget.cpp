// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/ContentPannel/GraphicPanelWidget.h"
#include "UI/ToolTip/DefaultToolTipWidget.h"
#include "UI/Pannel/ButtonPannel/OptionCycleButtonWidget.h"
#include "UI/Pannel/ButtonPannel/OptionSliderWidget.h"
#include "Subsystem/UserSettingSubsystem.h"
#include "Subsystem/PopupManageSubsystem.h"

void UGraphicPanelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mUserSettingSubsystem = GetGameInstance()->GetSubsystem<UUserSettingSubsystem>();
	checkf(mUserSettingSubsystem, TEXT("UUserSettingSubsystem Is Not Exit"))

	// Button Cache 
	TArray<TObjectPtr<UOptionCycleButtonWidget>> ChildButtons;
	TArray<TObjectPtr<UOptionSliderWidget>> ChildSlider;
	for (auto& W : SCB_OptionButtons->GetAllChildren())
	{
		UOptionCycleButtonWidget* Cbtn = Cast<UOptionCycleButtonWidget>(W);
		if (IsValid(Cbtn))
		{
			ChildButtons.Add(Cbtn);
		}
		else
		{
			UOptionSliderWidget* Slider = Cast<UOptionSliderWidget>(W);
			if (IsValid(Slider))
			{
				ChildSlider.Add(Slider);
			}
		}
	}

	BindOnHoverded_Button(ChildButtons);
	BindOnUnHoverded_Button(ChildButtons);

	BindOnHoverded_Slider(ChildSlider);
	BindOnUnHoverded_Slider(ChildSlider);


	UpdateButtons();
	BindUserSettingFunc();
}

void UGraphicPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

bool UGraphicPanelWidget::CheckIsAnySettingDirty()
{
	//*mUserSettingSubsystem->GetIsDirty()*/
	// PIE 모드에서 적용이 안될수 있어서 일단 직접 값을 비교하는 방식으로 변경

	if (/*mUserSettingSubsystem->GetIsDirty()*/
		CurrentScreenResolution != NewScreenResolution
		|| CurrentWindowMode != NewWindowMode
		|| CurrentVerticalSync != NewVerticalSync
		|| CurrentDynamicResolution != NewDynamicResolution
		|| CurrentResolutionScale != NewResolutionScale
		|| CurrentFrameRateLimit != NewFrameRateLimit
		|| CurrentOverallQuality != NewOverallQuality
		|| CurrentAntiAliasingQuality != NewAntiAliasingQuality
		|| CurrentFoliageQuality != NewFoliageQuality
		|| CurrentTextureQuality != NewTextureQuality
		|| CurrentShadingQuality != NewShadingQuality
		|| CurrentShadowQuality != NewShadowQuality
		|| CurrentReflectionsQuality != NewReflectionQuality
		|| CurrentPostProcessQuality != NewPostProcessQuality
		|| CurrentVisualEffectQuality != NewVisualEffectQuality
		|| CurrentViewDistanceQuality != NewViewDistanceQuality
		|| CurrentGlobalIlluminationQuality != NewGlobalIlluminationQuality)
	{
		bHasChanges = true;
	}

	return bHasChanges;

	
}

void UGraphicPanelWidget::Back()
{
	// Back 은 내부에서 처리후 최종적으로는 세팅옵션이 닫혀야한다(제거되어야한다)
	// 어떠한 변경점이 존재한다면, 타이머를 제시하고 적용시킬것인지 폐기시킬것인지 선택.

	// 변경사항이 존재
	if (bHasChanges)
	{
		// 해상도나 스크린 모드가 변경되었는지 확인
		bIsChangedResolution = mUserSettingSubsystem->GetIsScreenResolutionDirty();
		bIsChangedScreenMode = mUserSettingSubsystem->GetIsFullscreenModeDirty();

		UPopupManageSubsystem* PopupManageSubsystem = GetGameInstance()->GetSubsystem<UPopupManageSubsystem>();
		if (IsValid(PopupManageSubsystem))
		{
			FPopupInfo PopupInfo;
			PopupInfo.Title = FText::FromString(TEXT("Dont Save UserInfo"));
			PopupInfo.Script = FText::FromString(TEXT("Want save Click confirm. Isnt Click cancle"));
			PopupInfo.bOnlyConfirm = false;
			PopupInfo.bUserTimer = true;
			PopupInfo.OnConfirmDelegate.AddDynamic(this, &UGraphicPanelWidget::Callback_ConfirmBack);
			PopupInfo.OnCancleDelegate.AddDynamic(this, &UGraphicPanelWidget::Callback_CancleBack);


			PopupManageSubsystem->CreatePopup(PopupInfo);
		}
	}
	// 변경사항이 존재하지 않으면, 설정 위젯을 제거한다.
	else
	{
		RemoveFromParent();
	}
}

void UGraphicPanelWidget::ApplyChange()
{
	// 세팅을 저장하고, 해당페이지 유지

	// 1. 어플라이를 누르면 해상도를 제외한 "모든" 세팅은 저장된다.
	// 2. 팝업에서 Cancle 시, "해상도" 관련 옵션만 롤백된다.


	// 변경사항이 존재하지 않는다면, 아무것도 하지 않고 종료 
	if (false == bHasChanges)
	{
		return;
	}

	// 해상도 관련 옵션이 변경됐을 경우에는, 적용할지 재차 확인하고, 취소시 기존의 해상도로 롤백.
	// 단, 해상도 이외의 옵션은 적용되고, 롤백시키지 않는다.
	if (mUserSettingSubsystem->GetIsScreenResolutionDirty()
		|| mUserSettingSubsystem->GetIsFullscreenModeDirty())
	{
		// 행상도 및 스크린 모드를 적용시킬 것인지 재차 확인
		UPopupManageSubsystem* PopupManageSubsystem = GetGameInstance()->GetSubsystem<UPopupManageSubsystem>();
		if (IsValid(PopupManageSubsystem))
		{
			FPopupInfo PopupInfo;
			PopupInfo.Title = FText::FromString(TEXT("Dont Save UserInfo"));
			PopupInfo.Script = FText::FromString(TEXT("Want save Click confirm. Isnt Click cancle"));
			// 여기서 작은 글씨를 팝업에 추가해서 
			// 취소를 누른다해도 해상도 제외 다른 옵션들은 적용이됩니다. 이런거 추가해줘야함

			PopupInfo.bUserTimer = true;
			PopupInfo.OnConfirmDelegate.AddDynamic(this, &UGraphicPanelWidget::Callback_ConfirmApplyChange);
			PopupInfo.OnCancleDelegate.AddDynamic(this, &UGraphicPanelWidget::Callback_CancleApplyChange);
		
			PopupManageSubsystem->CreatePopup(PopupInfo);
		}

		mUserSettingSubsystem->CacheLastConfirmVideoMode();
		mUserSettingSubsystem->ApplySettings(false);
	}
	else
	{
		mUserSettingSubsystem->ApplyNonResolutionSettings();
		mUserSettingSubsystem->SaveSettings();
		UpdateButtons();
	}

	bHasChanges = false;
}

void UGraphicPanelWidget::Reset()
{
	mUserSettingSubsystem->SetToDefaults();
	// SetToDefaults 시, 모든 값이 기본으로 돌아가면 3 레벨의 초기값을 가지지만, 
	// 랜더퀄리티는 0 으로 초기화되면서, overall 쪽에 표시가 -1 (커스텀) 으로 된다.
	// 이를 방지하기 위해 해당 함수를 추가한다.
	mUserSettingSubsystem->SetResolutionScaleValue(100.f);
	mUserSettingSubsystem->ApplySettings(false);
	mUserSettingSubsystem->SaveSettings();
	UpdateButtons();
}

void UGraphicPanelWidget::Confirm()
{
}

void UGraphicPanelWidget::Revert()
{
}

void UGraphicPanelWidget::BindOnHoverded_Button(TArray<TObjectPtr<UOptionCycleButtonWidget>>& ButtonArr)
{
	for (auto& Btn : ButtonArr)
	{
		Btn->OnHovered().AddLambda([this, Btn]() {
			OnHoveredCallback_Button(Btn);
			});
	}
}

void UGraphicPanelWidget::BindOnUnHoverded_Button(TArray<TObjectPtr<UOptionCycleButtonWidget>>& ButtonArr)
{
	for (auto& Btn : ButtonArr)
	{
		Btn->OnUnhovered().AddLambda([this, Btn]() {
			OnUnHoveredCallback_Button(Btn);
			});
	}
}

void UGraphicPanelWidget::BindOnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr)
{
	for (auto& Slider : SliderArr)
	{
		Slider->OnHovered().AddLambda([this, Slider]() {
			OnHoveredCallback_Slider(Slider);
			});
	}
}

void UGraphicPanelWidget::BindOnUnHoverded_Slider(TArray<TObjectPtr<UOptionSliderWidget>>& SliderArr)
{
	for (auto& Slider : SliderArr)
	{
		Slider->OnUnhovered().AddLambda([this, Slider]() {
			OnUnHoveredCallback_Slider(Slider);
			});
	}
}

void UGraphicPanelWidget::OnHoveredCallback_Slider(UOptionSliderWidget* Slider)
{
	if (IsValid(Tooltip_OptionDesc))
	{
		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
		Tooltip_OptionDesc->SetDescript(Slider->mSetting.ButtonDescript);
	}
}

void UGraphicPanelWidget::OnUnHoveredCallback_Slider(UOptionSliderWidget* Slider)
{
	if (IsValid(Tooltip_OptionDesc))
	{
		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UGraphicPanelWidget::OnHoveredCallback_Button(UOptionCycleButtonWidget* Button)
{
	if (IsValid(Tooltip_OptionDesc))
	{
		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
		Tooltip_OptionDesc->SetDescript(Button->mSetting.ButtonDescript);
	}
}

void UGraphicPanelWidget::OnUnHoveredCallback_Button(UOptionCycleButtonWidget* Button)
{
	if (IsValid(Tooltip_OptionDesc))
	{
		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGraphicPanelWidget::UpdateButtons()
{
	/**
	* 현재 문제점
	* 1. 화면 해상도가 정확하게 맞아떨어지지 않음 
	* 2. 리셋시점에 오버롤값이 커스텀으로 설정되어있음
	*/

	if (Btn_ScreenResolution)
	{
		TArray<FText>& ResolutionValues = Btn_ScreenResolution->mSetting.StateValues; 
		ResolutionValues.Empty();
		TArray<FIntPoint> Resolutions;
		bool bExist = mUserSettingSubsystem->GetSupportedFullscreenResolutions(Resolutions);
		FIntPoint CurrentResolution = mUserSettingSubsystem->GetScreenResolution();
		FText CurrentResolutionText = mUserSettingSubsystem->ConvertResolutionToText(mUserSettingSubsystem->GetScreenResolution());
		
		if (bExist)
		{
			for (const auto& Resolution : Resolutions)
			{
				ResolutionValues.Add(mUserSettingSubsystem->ConvertResolutionToText(Resolution));
			}
		}
		else
		{
			ResolutionValues.Add(CurrentResolutionText);
		}

		// FTEXT 가 == 을 지원하지 않아서 아래와 같은 방법으로 대체
		int32 FindIdx = ResolutionValues.IndexOfByPredicate([&](const FText& InText)
				{
					return InText.EqualTo(CurrentResolutionText);
				});
		Btn_ScreenResolution->ChangeStateValue(FindIdx);

		NewScreenResolution = FindIdx;
		CurrentScreenResolution = NewScreenResolution;
	}
	 
	if (Btn_WindowMode)
	{
		EWindowMode::Type WindowModeType = mUserSettingSubsystem->GetFullscreenMode();
		Btn_WindowMode->ChangeStateValue(WindowModeType);

		NewWindowMode = WindowModeType;
		CurrentWindowMode = NewWindowMode;
	}

	if (Sl_ResolutionScale)
	{
		float CurrentScaleNormalized, CurrentScaleValue, MinScaleValue, MaxScaleValue;
		mUserSettingSubsystem->GetResolutionScaleInfo(CurrentScaleNormalized, CurrentScaleValue, MinScaleValue, MaxScaleValue);

		NewResolutionScale = CurrentScaleValue;
		CurrentResolutionScale = NewResolutionScale;

		Sl_ResolutionScale->ChangeStateValue(CurrentResolutionScale);
	}

	if (Btn_DynamicResolution)
	{
		int Idx = 0;
		bool bIsDynamicResolutionEnabled = mUserSettingSubsystem->GetIsDynamicResolutionEnabled();
		if (bIsDynamicResolutionEnabled)
		{
			Idx = 1;
		}

		Btn_DynamicResolution->ChangeStateValue(Idx);
		NewDynamicResolution = Idx;
		CurrentDynamicResolution = NewDynamicResolution;
	}

	if (Btn_VerticalSync)
	{
		int Idx = 0;
		bool bIsVsyncEnabled = mUserSettingSubsystem->GetIsVsyncEnabled();
		if (bIsVsyncEnabled)
		{
			Idx = 1;
		}

		Btn_VerticalSync->ChangeStateValue(Idx);
		NewVerticalSync = Idx;
		CurrentVerticalSync = NewVerticalSync;
	}

	if (Btn_FrameRateLimit)
	{
		int Idx = 0;
		float CurrentFrameLimit = mUserSettingSubsystem->GetFrameRateLimit();
		if (30.f == CurrentFrameLimit)
		{
			Idx = 0;
		}
		else if (60.f  == CurrentFrameLimit)
		{
			Idx = 1;
		}
		else if (120.f == CurrentFrameLimit)
		{
			Idx = 2;
		}
		else
		{
			Idx = 3;
		}
		
		Btn_FrameRateLimit->ChangeStateValue(Idx);
		NewFrameRateLimit = Idx;
		CurrentFrameRateLimit = NewFrameRateLimit;

	}

	if (Btn_OverallQuality)
	{
		NewOverallQuality = mUserSettingSubsystem->GetOverallScalabilityLevel();
		CurrentOverallQuality = NewOverallQuality;
		Btn_OverallQuality->ChangeStateValue(CurrentOverallQuality);
	}

	if (Btn_AntiAliasingQuality)
	{
		NewAntiAliasingQuality = mUserSettingSubsystem->GetAntiAliasingQuality();
		CurrentAntiAliasingQuality = NewAntiAliasingQuality;
		Btn_AntiAliasingQuality->ChangeStateValue(CurrentAntiAliasingQuality);
	}

	if (Btn_FoliageQuality)
	{
		NewFoliageQuality = mUserSettingSubsystem->GetFoliageQuality();
		CurrentFoliageQuality = NewFoliageQuality;
		Btn_FoliageQuality->ChangeStateValue(CurrentFoliageQuality);
	}

	if (Btn_TextureQuality)
	{
		NewTextureQuality = mUserSettingSubsystem->GetTextureQuality();
		CurrentTextureQuality = NewTextureQuality;
		Btn_TextureQuality->ChangeStateValue(CurrentTextureQuality);
	}

	if (Btn_ShadingQuality)
	{
		NewShadingQuality = mUserSettingSubsystem->GetShadingQuality();
		CurrentShadingQuality = NewShadingQuality;
		Btn_ShadingQuality->ChangeStateValue(CurrentShadingQuality);
	}

	if (Btn_ShadowQuality)
	{
		NewShadowQuality = mUserSettingSubsystem->GetShadowQuality();
		CurrentShadowQuality = NewShadowQuality;
		Btn_ShadowQuality->ChangeStateValue(CurrentShadowQuality);
	}

	if (Btn_ReflectionsQuality)
	{
		NewReflectionQuality = mUserSettingSubsystem->GetReflectionQuality();
		CurrentReflectionsQuality = NewReflectionQuality;
		Btn_ReflectionsQuality->ChangeStateValue(CurrentReflectionsQuality);
	}

	if (Btn_PostProcessQuality)
	{
		NewPostProcessQuality = mUserSettingSubsystem->GetPostProcessQuality();
		CurrentPostProcessQuality = NewPostProcessQuality;
		Btn_PostProcessQuality->ChangeStateValue(CurrentPostProcessQuality);
	}

	if (Btn_VisualEffectQuality)
	{
		NewVisualEffectQuality = mUserSettingSubsystem->GetVisualEffectQuality();
		CurrentVisualEffectQuality = NewVisualEffectQuality;
		Btn_VisualEffectQuality->ChangeStateValue(CurrentVisualEffectQuality);
	}

	if (Btn_ViewDistanceQuality)
	{
		NewViewDistanceQuality = mUserSettingSubsystem->GetViewDistanceQuality();
		CurrentViewDistanceQuality = NewViewDistanceQuality;
		Btn_ViewDistanceQuality->ChangeStateValue(CurrentViewDistanceQuality);
	}

	if (Btn_GlobalIlluminationQuality)
	{
		NewGlobalIlluminationQuality = mUserSettingSubsystem->GetGlobalIlluminationQuality();
		CurrentGlobalIlluminationQuality = NewGlobalIlluminationQuality;
		Btn_GlobalIlluminationQuality->ChangeStateValue(CurrentGlobalIlluminationQuality);
	}
}

void UGraphicPanelWidget::BindUserSettingFunc()
{
	if (Btn_ScreenResolution)
	{
		Btn_ScreenResolution->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleScreenResolution);
	}
	
	if (Btn_WindowMode)
	{
		Btn_WindowMode->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleWindowMode);
	}
	
	if (Sl_ResolutionScale)
	{
		Sl_ResolutionScale->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleResolutionScaleValue);
	}
	
	if (Btn_DynamicResolution)
	{
		Btn_DynamicResolution->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleDynamicResolution);
	}
	
	if (Btn_VerticalSync)
	{
		Btn_VerticalSync->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleVSync);
	}
	
	if (Btn_FrameRateLimit)
	{
		Btn_FrameRateLimit->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleFrameRateLimit);
	}
	
	if ( Btn_OverallQuality)
	{
		Btn_OverallQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleOverallScalabilityLevel);
	}
	
	if (Btn_AntiAliasingQuality)
	{
		Btn_AntiAliasingQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleAntiAliasingQuality);
	}
	
	if ( Btn_FoliageQuality)
	{
		Btn_FoliageQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleFoliageQuality);
	}
	
	if (Btn_TextureQuality)
	{
		Btn_TextureQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleTextureQuality);
	}
	
	if (Btn_ShadingQuality)
	{
		Btn_ShadingQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleShadingQuality);
	}
	
	if ( Btn_ShadowQuality)
	{
		Btn_ShadowQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleShadowQuality);
	}
	
	if ( Btn_ReflectionsQuality)
	{
		Btn_ReflectionsQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleReflectionQuality);
	}
	
	if ( Btn_PostProcessQuality)
	{
		Btn_PostProcessQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandlePostProcessQuality);
	}
	
	if (Btn_VisualEffectQuality)
	{
		Btn_VisualEffectQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleVisualEffectQuality);
	}
	
	if ( Btn_ViewDistanceQuality)
	{
		Btn_ViewDistanceQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleViewDistanceQuality);
	}
	
	if (Btn_GlobalIlluminationQuality)
	{
		Btn_GlobalIlluminationQuality->mOnOptionChanged.AddDynamic(this, &UGraphicPanelWidget::HandleGlobalIlluminationQuality);
	}
}

void UGraphicPanelWidget::ToggleTooltip(UWidget* Widget, bool bEnable)
{
	//if (IsValid(Tooltip_OptionDesc))
	//{
	//	if (bEnable)
	//	{
	//		Tooltip_OptionDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
	//		Tooltip_OptionDesc->SetDescript(Button->mSetting.ButtonDescript);

	//		FGeometry ButtonGeo = Button->GetCachedGeometry();
	//		FGeometry CanvasGeo = GetCachedGeometry(); // 현재 위젯 전체의 Geometry (Canvas 기준)

	//		// 버튼의 절대좌표를 Canvas 기준 Local로 변환
	//		FVector2D CanvasSpacePos = CanvasGeo.AbsoluteToLocal(ButtonGeo.GetAbsolutePosition());
	//		FVector2D ButtonSize = ButtonGeo.GetLocalSize();

	//		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, *ButtonSize.ToString());

	//		// 툴팁 이동
	//		if (UCanvasPanelSlot* TooltipSlot = Cast<UCanvasPanelSlot>(Tooltip_OptionDesc->Slot))
	//		{
	//			TooltipSlot->SetPosition(FVector2D(20.f, CanvasSpacePos.Y + ButtonSize.Y / 2.f));
	//		}
	//	}
	//	else
	//	{
	//		if (IsValid(Tooltip_OptionDesc))
	//		{
	//			Tooltip_OptionDesc->SetVisibility(ESlateVisibility::Collapsed);
	//		}
	//	}
	//}
}

void UGraphicPanelWidget::Callback_ConfirmBack()
{
	if (bIsChangedResolution || bIsChangedScreenMode)
	{
		mUserSettingSubsystem->ApplySettings(false);
		mUserSettingSubsystem->ConfirmVideoMode();
		mUserSettingSubsystem->SaveSettings();
	}
	else
	{
		mUserSettingSubsystem->ApplyNonResolutionSettings();
	}

	RemoveFromParent();
}

void UGraphicPanelWidget::Callback_CancleBack()
{
	mUserSettingSubsystem->LoadSettings(false);
	RemoveFromParent();
}

void UGraphicPanelWidget::Callback_ConfirmApplyChange()
{
	mUserSettingSubsystem->ConfirmVideoMode();
	mUserSettingSubsystem->SaveSettings();
	UpdateButtons();
}

void UGraphicPanelWidget::Callback_CancleApplyChange()
{
	mUserSettingSubsystem->RevertVideoMode(true);
	mUserSettingSubsystem->ApplyResolutionSettings(false);
	mUserSettingSubsystem->SaveSettings();
	UpdateButtons();
}

void UGraphicPanelWidget::HandleScreenResolution(int32 StateValue)
{
	const FText& ResolutionText = Btn_ScreenResolution->mSetting.StateValues[StateValue];
	const FString& ResolutionString = ResolutionText.ToString();
	FString Left, Right;
	ResolutionString.Split(TEXT("x"), &Left, &Right); // 첫 번째 x 기준

	int32 x = (FCString::Atoi(*Left));
	int32 Y= (FCString::Atoi(*Right));

	mUserSettingSubsystem->SetScreenResolution(FIntPoint(FCString::Atoi(*Left), FCString::Atoi(*Right)));
	NewScreenResolution = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleWindowMode(int32 StateValue)
{
	mUserSettingSubsystem->SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(StateValue));
	NewWindowMode = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleResolutionScaleValue(float StateValue)
{
	mUserSettingSubsystem->SetResolutionScaleValue(StateValue);
	NewResolutionScale = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleDynamicResolution(int32 StateValue)
{
	bool bEnable = false;
	switch (StateValue)
	{
	case 0:
		bEnable = false;
		break;
	case 1:
		bEnable = true;
		break;
	}

	mUserSettingSubsystem->SetDynamicResolutionEnabled(bEnable);
	NewDynamicResolution = StateValue;

	Broadcast_Settings();
}


void UGraphicPanelWidget::HandleVSync(int32 StateValue)
{
	bool bEnable = false;
	switch (StateValue)
	{
	case 0:
		bEnable = false;
		break;
	case 1:
		bEnable = true;
		break;
	}

	mUserSettingSubsystem->SetVsyncEnabled(bEnable);
	NewVerticalSync = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleFrameRateLimit(int32 StateValue)
{
	float FrameRate = 0.f;
	switch (StateValue)
	{
	case 0:
		FrameRate = 30.f;
		break;
	case 1:
		FrameRate = 60.f;
		break;
	case 2:
		FrameRate = 90.f;
		break;
	case 3:
		FrameRate = 0.f;
		break;
	}

	mUserSettingSubsystem->SetFrameRateLimit(FrameRate);
	NewFrameRateLimit = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleOverallScalabilityLevel(int32 StateValue)
{
	mUserSettingSubsystem->SetOverallScalabilityLevel(StateValue);
	NewOverallQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleAntiAliasingQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetAntiAliasingQuality(StateValue);
	NewAntiAliasingQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleFoliageQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetFoliageQuality(StateValue);
	NewFoliageQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleTextureQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetTextureQuality(StateValue);
	NewTextureQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleShadingQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetShadingQuality(StateValue);
	NewShadingQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleShadowQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetShadowQuality(StateValue);
	NewShadowQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleReflectionQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetReflectionQuality(StateValue);
	NewReflectionQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandlePostProcessQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetPostProcessQuality(StateValue);
	NewPostProcessQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleVisualEffectQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetVisualEffectQuality(StateValue);
	NewVisualEffectQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleViewDistanceQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetViewDistanceQuality(StateValue);
	NewViewDistanceQuality = StateValue;

	Broadcast_Settings();
}

void UGraphicPanelWidget::HandleGlobalIlluminationQuality(int32 StateValue)
{
	mUserSettingSubsystem->SetGlobalIlluminationQuality(StateValue);
	NewGlobalIlluminationQuality = StateValue;

	Broadcast_Settings();
}

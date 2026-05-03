// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Macro/Log.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Components/TreeView.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/SizeBoxSlot.h"
#include "Components/CheckBox.h"
#include "Components/Spacer.h"
#include "Components/SpinBox.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "CommonUI/Public/CommonButtonBase.h"
#include "CommonUI/Public/CommonRotator.h"
#include "CommonUI/Public/CommonTextBlock.h"
#include "CommonUI/Public/CommonAnimatedSwitcher.h"


#include "UIInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionChanged, int32, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionChanged_float, float, Value);


enum class EWidgetMouseState : uint8
{
	None,
	Hovered,
	Select
};

UENUM(BlueprintType, meta = (ScriptName = "EUserSettingCategory"))
enum class EUserSettingCategory : uint8
{
	None,
	Graphic,
	Input,
	Sound,
};

USTRUCT(BlueprintType)
struct FUserSettingCategory : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EUserSettingCategory UserSettingCategory = EUserSettingCategory::None;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> DTOptionTable = nullptr;
};

USTRUCT(BlueprintType)
struct FCommonOption : public FTableRowBase
{
	GENERATED_BODY()

	// 버튼에 표시될 이름 or enum 으로 표시
	UPROPERTY(EditAnywhere)
	FText OptionName;

	UPROPERTY(EditAnywhere)
	bool UseCycle = true;

	// 사이클일 경우 값 - 표시명) ex 0 - Low, 1 - middle, 2 - high, 3 - epic, 4 - cinematic
	UPROPERTY(EditAnywhere)
	TMap<int, FText> CycleOptionsValue =
	{
		{0 , FText::FromString("Low")},
		{1 , FText::FromString("Middle")},
		{2 , FText::FromString("High")},
		{3 , FText::FromString("Epic")},
		{4 , FText::FromString("Cinematic")}
	};

	UPROPERTY(EditAnywhere)
	bool UseSlider = false;

	UPROPERTY(EditAnywhere)
	float Value = 0.f;
};

USTRUCT(BlueprintType)
struct FSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText ButtonName;

	UPROPERTY(EditAnywhere)
	TArray<FText> StateValues;

	UPROPERTY(EditAnywhere)
	int32 DefaultIdx = 0;

	UPROPERTY(EditAnywhere)
	FText ButtonDescript;
};

USTRUCT(BlueprintType)
struct FSettings_Slider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText ButtonName;

	UPROPERTY(EditAnywhere)
	FText ButtonDescript;
};

UENUM(BlueprintType)
enum class EGlobalColors : uint8
{
	None,
	OverallColorTone1,
	OverallColorTone2,
	OverallColorContrast,
	OverallColorFont,
	OverallColorFontContrast,
	OverallColorFontWhite,
};


USTRUCT(BlueprintType)
struct FTextStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonTextStyle> TextStyle = nullptr;

	UPROPERTY(EditAnywhere)
	bool UseTextColorOverride = false;

	UPROPERTY(EditAnywhere)
	FLinearColor TextColorOverride = FLinearColor();

	UPROPERTY(EditAnywhere)
	EGlobalColors TextColorGlobalType = EGlobalColors::None;

	UPROPERTY(EditAnywhere)
	float TextOpacity = 0.f;
};

USTRUCT(BlueprintType)
struct FButtonTextStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FTextStyle Noraml;

	UPROPERTY(EditAnywhere)
	FTextStyle Hover;

	UPROPERTY(EditAnywhere)
	FTextStyle Press;

	UPROPERTY(EditAnywhere)
	FTextStyle SelectedNormal;

	UPROPERTY(EditAnywhere)
	FTextStyle SelectedHover;

	UPROPERTY(EditAnywhere)
	FTextStyle Disabled;
};





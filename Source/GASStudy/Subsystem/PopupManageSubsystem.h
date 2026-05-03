// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Popup/PopupWidget.h"
#include "PopupManageSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FPopupInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Script;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnConfirm OnConfirmDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnCancle OnCancleDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOnlyConfirm = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUserTimer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingTime = 10;

	FPopupInfo() {}

	FPopupInfo(FText InTitle, FText InScript, FOnConfirm InOnConfirmDelegate, FOnCancle InOnCancleDelegate, bool InbOnlyConfirm)
		: Title(InTitle)
		, Script(InScript)
		, OnConfirmDelegate(InOnConfirmDelegate)
		, OnCancleDelegate(InOnCancleDelegate)
		, bOnlyConfirm(InbOnlyConfirm)
	{
	}
};

// 팝업 팩토리 
UENUM(BlueprintType)
enum class EPopupType : uint8
{
	Tutorial_Parry,
	Tutorial_Finisher,
};

class CPopupFactory 
{
public:
	static FPopupInfo CreatePopup(EPopupType Type)
	{
		switch (Type)
		{
		case EPopupType::Tutorial_Parry:
			return CreateParryPopup();

		case EPopupType::Tutorial_Finisher:
			return CreateFinisher();
		}

		return FPopupInfo();
	}

private:
	static FPopupInfo CreateParryPopup()
	{
		FPopupInfo Info;

		Info.Title = FText::FromString(TEXT("Parry"));
		Info.Script = FText::FromString(TEXT("If you block at the exact moment of an enemy's attack, you can break their stance."));
		Info.bOnlyConfirm = true;

		return Info;
	}

	static FPopupInfo CreateFinisher()
	{
		FPopupInfo Info;

		Info.Title = FText::FromString(TEXT("Finisher"));
		Info.Script = FText::FromString(TEXT("Approach from outside the enemy's vision, or fully deplete their groggy gauge to instantly kill them regardless of their remaining health."));
		Info.bOnlyConfirm = true;

		return Info;
	}
};

/**
 * 
 */
UCLASS()
class GASSTUDY_API UPopupManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPopupManageSubsystem();

public:
	/**
	* 팝업 위젯이 제거되는 순간 NativeDestruct 에서 호출
	*/
	UFUNCTION(BlueprintCallable)
	bool CreatePopupFactory(EPopupType PopupType);

	UFUNCTION(BlueprintCallable)
	bool CreatePopup(const FPopupInfo& PopupInfo);

	void PopupClosed();

private:
	void CreatePopupWidget();

	/**
	* 팝업 Cancle시 기본적으로 적용되는 델리게이트
	*/
	UFUNCTION()
	void Callback_ClosedPopup();

	void FocusPopup(UUserWidget* PopupWidget);
	
	UFUNCTION()
	void ClearFocusPopup();

private:
	UPROPERTY()
	TSubclassOf<UPopupWidget> mPopupWidgetClass;

	UPROPERTY()
	TSubclassOf<UTimerPopupWidget> mTimerPopupWidgetClass;


private:
	// 팝업은 항상 한개만 작동한다. 
	TQueue<FPopupInfo> mPopupQue;

	bool bIsPopupOpened = false;

	bool bCacheShowMouseCursor;
};



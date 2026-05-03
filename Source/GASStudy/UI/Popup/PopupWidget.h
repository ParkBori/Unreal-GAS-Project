// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "CommonActivatableWidget.h"
#include "UI/UIInfo.h"
#include "PopupWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirm);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeatroy);

class UBaseButtonWidget;

/**
 * 
 */
UCLASS()
class GASSTUDY_API UPopupWidget : public UUserWidget/*UCommonActivatableWidget*/
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;

public: 
	void Init(FText Title, FText Script, FOnConfirm OnConfirmDelegate, FOnCancle OnCancleDelegate, bool bOnlyConfirm = false);
	
	void CreateAndAddButton(FText ButtonName, TFunction<void()> CallFunc);

protected:
	virtual void OnClickedCallback_Confirm();
	virtual void OnClickedCallback_Cancle();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_Script;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Buttons;

	UPROPERTY()
	TArray<TObjectPtr<UButton>> mCacheButton;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	FText mTitleText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	FText mScriptText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TSubclassOf<UBaseButtonWidget> mButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TSubclassOf<UCommonTextStyle> mHoverStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TSubclassOf<UCommonTextStyle> mNormalStyle;

public:
	FOnConfirm mOnConfirm;
	FOnCancle mOnCancle;
	FOnDeatroy mOnDestroy;
};

UCLASS()
class GASSTUDY_API UTimerPopupWidget: public UPopupWidget/*UCommonActivatableWidget*/
{
	GENERATED_BODY()



protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetRemainingTime(int32 TimerTime)
	{
		mRemainingTime = TimerTime;
	}

private:
	void SetText(int32 Seconds);

private:
	FIntPoint ConvertSecondsToTime(int Seconds);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_Count;

private:
	int32  mRemainingTime = 10;
	float mAcc = 0.f;
};

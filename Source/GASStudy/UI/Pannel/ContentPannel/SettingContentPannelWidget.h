// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Pannel/ContentPannel/ContentPannelWidget.h"
#include "UI/UIInfo.h"
#include "Interface/ContentPanenlUIInterface.h"
#include "SettingContentPannelWidget.generated.h"

// 어떤 옵션이 변경되었을 때 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnySettingChanged);
// 변경된 옵션이 기존의 값으로 돌가았을 때 호출되는 델리게이트 (리커버리)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnySettingUnChanged);


class UOptionCycleButtonWidget;

/**
 * 
 */
UCLASS()
class GASSTUDY_API USettingContentPannelWidget 
	: public UContentPannelWidget
	, public IContentPanenlUIInterface
{
	GENERATED_BODY()

protected:
	virtual bool CheckIsAnySettingDirty() { return false; };

	// IContentPanenlUIInterface
	virtual void Back() override;
	virtual void ApplyChange() override {};
	virtual void Reset() override {};

	// 설정 확정
	virtual void Confirm() {};
	// 설정 복구
	virtual void Revert() {};

	void Broadcast_Settings();

protected:
	void BroadCast_OnAnySettingChanged()
	{
		if (mOnAnySettingChanged.IsBound())
		{
			mOnAnySettingChanged.Broadcast();
		}
	}

	void BroadCast_OnAnySettingUnChanged()
	{
		if (mOnAnySettingUnChanged.IsBound())
		{
			mOnAnySettingUnChanged.Broadcast();
		}
	}


protected:
	bool bHasChanges = false;

	UPROPERTY(BlueprintAssignable)
	FOnAnySettingChanged mOnAnySettingChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAnySettingUnChanged mOnAnySettingUnChanged;
};

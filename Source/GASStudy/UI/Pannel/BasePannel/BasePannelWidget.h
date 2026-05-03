// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/UIInfo.h"
#include "BasePannelWidget.generated.h"


class UContentPannelWidget;

/**
 * 
 */
UCLASS()
class GASSTUDY_API UBasePannelWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	// virtual void NativePreConstruct();
	virtual void NativeConstruct() override;
	// virtual void NativeDestruct();
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	

public:
	UFUNCTION(BlueprintCallable)
	void InitWidget(TSubclassOf<UContentPannelWidget> ContentWidget)
	{
		mContentClass = ContentWidget;
	}

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> OV_Content;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	TSubclassOf <UContentPannelWidget> mContentClass;

protected:
	UPROPERTY()
	TObjectPtr <UUserWidget> mContentWidget;
};

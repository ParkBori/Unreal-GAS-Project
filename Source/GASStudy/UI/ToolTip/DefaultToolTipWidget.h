// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "DefaultToolTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UDefaultToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetDescript(const FText& Descript);
	float GetHeight();

protected:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UCommonTextBlock> CT_Descript;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};

class UInventoryComponent_V2;

UCLASS()
class GASSTUDY_API UItemToolTipWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetDescript(const FString& Descript);
	void Reset();
	void AddOption(const FName& OptionName, float Value, bool bIsPercent = false);

	UFUNCTION(BlueprintCallable)
	void UpdateToolTip(const FToolTipInfo& ToolTipInfo);

	UFUNCTION(BlueprintCallable)
	void UnHoveredEvent(bool bValue);

private:
	void SetPosition(const FVector2D& Position, const FVector2D& SlotSize);


public:
	UFUNCTION()
	void UpdateToolTipData(const FToolTipData& ToolTipInfo);

public:
	UFUNCTION()
	void UpdatePositionFromSlot(const FGeometry& SlotGeo);

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UInventoryComponent_V2> mInventoryComponent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_Option;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CT_Descript;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonTextStyle> mTextStyle;
};


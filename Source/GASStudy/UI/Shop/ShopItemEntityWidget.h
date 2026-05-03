// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Shop/ShopData.h"
#include "ShopItemEntityWidget.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmountChanged, int32);


static constexpr int32 MAX_AMOUNT = 999;

UCLASS(BlueprintType)
class GASSTUDY_API UShopItemItemData : public UObject
{
	GENERATED_BODY()

public:
	static UShopItemItemData* Create(const FShopItemData& InData)
	{
		UShopItemItemData* NewObj = NewObject<UShopItemItemData>();
		NewObj->ItemData = InData;
		return NewObj;
	}

	void AddAmount(int32 Amount)
	{
		ItemData.Amount  = FMath::Clamp(ItemData.Amount + Amount, 0, MAX_AMOUNT);
		mOnAmountChanged.Broadcast(ItemData.Amount);
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShopItemData ItemData;

	FOnAmountChanged mOnAmountChanged;
};

/**
 * 
 */
UCLASS()
class GASSTUDY_API UShopItemEntityWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

protected:
	virtual void NativeOnInitialized() override;

private:
	void UpdateSelectionVisuals(bool bIsSelected);

	void Callback_UpdateAmount(int32 NewAmount);
public:
	void Init();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Price;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Cover;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Background;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Amount;
};

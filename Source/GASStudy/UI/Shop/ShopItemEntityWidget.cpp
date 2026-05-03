// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopItemEntityWidget.h"

void UShopItemEntityWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UShopItemItemData* ShopItemData = Cast<UShopItemItemData>(ListItemObject))
	{
		// 위젯이 재사용될 때마다 새로운 데이터로 갱신
		Img_ItemIcon->SetBrushFromTexture(ShopItemData->ItemData.ItemIcon);
		TB_ItemName->SetText(ShopItemData->ItemData.ItemName);
		TB_Price->SetText(FText::AsNumber(ShopItemData->ItemData.ItemPrice));
		TB_Amount->SetText(FText::AsNumber(ShopItemData->ItemData.Amount));

		if (ShopItemData->ItemData.Amount <= 0)
		{
			Img_Cover->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	
		// 헬퍼 함수나 리스트뷰 참조를 통해 현재 선택 여부 확인
		bool bIsSelected = IsListItemSelected();

		// 3. 현재 상태에 맞게 배경색 초기화 (매우 중요!)
		UpdateSelectionVisuals(bIsSelected);

		ShopItemData->mOnAmountChanged.RemoveAll(this);
		ShopItemData->mOnAmountChanged.AddUObject(this, &UShopItemEntityWidget::Callback_UpdateAmount);
	}
}

void UShopItemEntityWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UShopItemEntityWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	UpdateSelectionVisuals(bIsSelected);
}

void UShopItemEntityWidget::UpdateSelectionVisuals(bool bIsSelected)
{
	if (Img_Background)
	{
		FLinearColor TargetColor = bIsSelected ?
			FLinearColor(0.848958f, 0.0f, 0.0f, 0.56f) :
			FLinearColor(1.0f, 1.0f, 1.0f, 0.56f);

		Img_Background->SetColorAndOpacity(TargetColor);
	}
}

void UShopItemEntityWidget::Callback_UpdateAmount(int32 NewAmount)
{
	if (!IsValid(TB_Amount))
	{
		return;
	}

	TB_Amount->SetText(FText::AsNumber(NewAmount));
}

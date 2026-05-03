// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopData.generated.h"


enum class EShopItemInteractResultType : uint8
{
	// Buy
	BUY_OUT_OF_STOCK			UMETA(ToolTip = "재고 부족"),
	BUY_NOT_ENOUGH_GOLD			UMETA(ToolTip = "돈 부족"),
	BUY_NOT_ENOUGH_SPACE		UMETA(ToolTip = "인벤토리 공간 부족"),
	BUY_SUCCESS					UMETA(ToolTip = "구매 성공"),

	// Buy
	SELL_OUT_OF_STOCK			UMETA(ToolTip = "판매 수량 부족"),
	SELL_SUCCESS				UMETA(ToolTip = "판매 성공"),
};

UENUM(BlueprintType)
enum class EShopItemListType : uint8
{
	None,
	Sell,
	Buy,
};

enum class EShopModeType : uint8
{
	None,
	Buy,
	Sell,
};


USTRUCT(BlueprintType)
struct FShopItemData
{
	GENERATED_BODY()


	FShopItemData() {}

	FShopItemData(
		UTexture2D* InIcon,
		FText InItemName,
		float InItemPrice,
		int32 InAmount,
		FGuid InSlotId) :
		ItemIcon(InIcon),
		ItemName(InItemName),
		ItemPrice(InItemPrice),
		Amount(InAmount),
		SlotID(InSlotId)
	{
	}


	UPROPERTY(BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite)
	float ItemPrice = 0.f;

	UPROPERTY(BlueprintReadWrite)
	int32 Amount = 0;

	UPROPERTY(BlueprintReadWrite)
	FGuid SlotID = FGuid();
};

static FText GetInteractResultMessage(EShopItemInteractResultType Result)
{
	static const TMap<EShopItemInteractResultType, FText> ResultMsg = {
		{ EShopItemInteractResultType::BUY_OUT_OF_STOCK,    FText::FromString(TEXT("자네가 원하는 만큼은 내게 없는 것 같군.")) },
		{ EShopItemInteractResultType::BUY_NOT_ENOUGH_GOLD, FText::FromString(TEXT("자네 돈이 부족하진 않은가? 제 값을 치루고 가져가게.")) },
		{ EShopItemInteractResultType::BUY_NOT_ENOUGH_SPACE, FText::FromString(TEXT("자네 이 이상은 들고다니느 것은 어렵겠는게.")) },
		{ EShopItemInteractResultType::BUY_SUCCESS,         FText::FromString(TEXT("고맙네. 훌륭한 선택이야.")) },
		{ EShopItemInteractResultType::SELL_OUT_OF_STOCK,        FText::FromString(TEXT("어찌 가진 거 이상을 팔겠다는건가?")) },
		{ EShopItemInteractResultType::SELL_SUCCESS,        FText::FromString(TEXT("고맙네. 다른 이에게 또다른 인연을..")) }
	};

	const FText* FoundMessage = ResultMsg.Find(Result);
	return FoundMessage ? *FoundMessage : FText::GetEmpty();
}
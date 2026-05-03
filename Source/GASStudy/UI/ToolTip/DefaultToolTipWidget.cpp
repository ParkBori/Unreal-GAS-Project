// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ToolTip/DefaultToolTipWidget.h"
#include "Component/InventoryComponent_V2.h"

#include "Subsystem/ItemSubsystem.h"


void UDefaultToolTipWidget::SetDescript(const FText& Descript)
{
	if (IsValid(CT_Descript))
	{
		CT_Descript->SetText(Descript);
	}
}

float UDefaultToolTipWidget::GetHeight()
{
	if (IsValid(SizeBox))
	{
		return SizeBox->GetHeightOverride();
	}

	return 0.f;
}



void UItemToolTipWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UItemToolTipWidget::SetDescript(const FString& Descript)
{
	if (IsValid(CT_Descript))
	{
		CT_Descript->SetText(FText::FromString(Descript));
	}
}

void UItemToolTipWidget::Reset()
{
	if (IsValid(VB_Option))
	{
		VB_Option->ClearChildren();
	}
}


void UItemToolTipWidget::AddOption(const FName& OptionName, float Value, bool bIsPercent)
{
	if (IsValid(CT_Descript))
	{
		FString ValueString = FString::SanitizeFloat(Value);
		if (bIsPercent)
		{
			ValueString += "%";
		}
		
		FString Option = "+ " + ValueString + "	" + OptionName.ToString();

		UCommonTextBlock* CTB_Option = NewObject<UCommonTextBlock>(VB_Option);
		CTB_Option->SetText(FText::FromString(Option));
		CTB_Option->SetStyle(mTextStyle);

		auto SL = VB_Option->AddChildToVerticalBox(CTB_Option);
		if (IsValid(SL))
		{
			SL->SetPadding(FMargin(3, 2, 3, 0));
		}
	}
}

void UItemToolTipWidget::UpdateToolTip(const FToolTipInfo& ToolTipInfo)
{
	if (ToolTipInfo.ItemName.IsNone())
	{
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
	
	// 위치 잡아줘야함


	// 아이템 이름
	CTB_ItemName->SetText(FText::FromName(ToolTipInfo.ItemName));

	// 아이템 설명 
	if (IsValid(CT_Descript))
	{
		FString EscriptMsg = "\"" + ToolTipInfo.ItemDescript + "\"";
		CT_Descript->SetText(FText::FromString(EscriptMsg));
	}
	
	// 옵션 슬롯 비우기
	VB_Option->ClearChildren();

	// 아이템 옵션 
	for (const auto& Option : ToolTipInfo.ItemOption)
	{
		AddOption(Option.Key, Option.Value);
	}

	SetPosition(ToolTipInfo.Position, ToolTipInfo.SlotSize);
}

void UItemToolTipWidget::UnHoveredEvent(bool bValue)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UItemToolTipWidget::SetPosition(const FVector2D& Position, const FVector2D& SlotSize)
{
	// 여기로 보내지는 위치는, 각 슬롯이 존재하는 캔버스 패널 기준 좌표가 나오게 된다
	// 이를 더 상위 패널을 기준으로 전환해주는게 필요

	const FGeometry& ParentGeo = GetParent()->GetCachedGeometry();
	const FVector2D& ParentSpaceSlotPos = ParentGeo.AbsoluteToLocal(Position);
	FVector2D Offset = { SlotSize.X, 0 };

	auto SL = Cast<UCanvasPanelSlot>(this->Slot);
	if (IsValid(SL))
	{
		SL->SetPosition(ParentSpaceSlotPos + Offset);
	}

	//SetRenderTranslation(ParentSpaceSlotPos + Offset);
}

void UItemToolTipWidget::UpdateToolTipData(const FToolTipData& ToolTipInfo)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	
	CTB_ItemName->SetText(FText::FromName(ToolTipInfo.ItemName));
	
	// 아이템 설명 
	if (IsValid(CT_Descript))
	{
		FString EscriptMsg = "\"" + ToolTipInfo.ItemDescript + "\"";
	
		CT_Descript->SetText(FText::FromString(EscriptMsg));
	}
	
	// 옵션 슬롯 비우기
	VB_Option->ClearChildren();
	
	// 아이템 옵션 
	for (const auto& Option : ToolTipInfo.ItemOptions)
	{
		AddOption(*Option.Key.GetName(), Option.Value);
	}
}

void UItemToolTipWidget::UpdatePositionFromSlot(const FGeometry& SlotGeo)
{
	FVector2D PixelPos;
	FVector2D ViewportPos;

	// 뷰포트 슬롯 위치
	USlateBlueprintLibrary::AbsoluteToViewport(
		GetWorld(),
		SlotGeo.GetAbsolutePosition(),
		PixelPos,
		ViewportPos
	);

	// 슬롯 사이즈
	FVector2D SlotSize = SlotGeo.GetLocalSize();

	// 뷰포트 크기
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// 슬롯 중심 x축 좌표
	float SlotCenterX = ViewportPos.X + SlotSize.X * 0.5f;
	// 화면의 중앙 위치
	float ScreenCenterX = ViewportSize.X * 0.5f;

	// 툴팁 크기 (중요!)
	FVector2D TooltipSize = GetDesiredSize();

	FVector2D TooltipPos = ViewportPos;

	if (SlotCenterX < ScreenCenterX)
	{
		// 슬롯이 화면 왼쪽 → 툴팁을 오른쪽에
		TooltipPos.X += SlotSize.X;
	}
	else
	{
		// 슬롯이 화면 오른쪽 → 툴팁을 왼쪽에
		TooltipPos.X -= TooltipSize.X;
	}

	// 위 아래 클램프
	TooltipPos.Y = FMath::Clamp(
		TooltipPos.Y,
		0.f,
		ViewportSize.Y - TooltipSize.Y
	);

	SetPositionInViewport(TooltipPos, false);
}
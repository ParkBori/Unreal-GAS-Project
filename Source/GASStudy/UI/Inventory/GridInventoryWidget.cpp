// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/GridInventoryWidget.h"
#include "UI/Inventory/GridItemSlotWidget.h"
#include "UI/Inventory/BaseSlotWidget.h"
#include "Tag/GameplayTag.h"
#include "Shared/Item/ItemInfo.h"

void UGridInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IsValid(UG_GridPanel) && IsValid(mBackgroundSlotClass))
	{
		for (int i = 0; i < INVENTORY_GRID_Y; ++i)
		{
			for (int j = 0; j < INVENTORY_GRID_X; ++j)
			{
				UGridItemSlotWidget* SlWidget = CreateWidget<UGridItemSlotWidget>(this, mBackgroundSlotClass);
				if (IsValid(SlWidget))
				{
					UG_GridPanel->AddChildToUniformGrid(SlWidget, i, j);
				}
			}
		}
	}

	mInvnetorySlotArr.SetNumZeroed(INVENTORY_GRID_Y * INVENTORY_GRID_X);


	UCanvasPanelSlot* CSL = Cast<UCanvasPanelSlot>(Highlight->Slot);
	if (IsValid(CSL))
	{
		CSL_Highlight = CSL;
	}

	if (GetParent())
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *GetParent()->GetName());
	}
}

void UGridInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

bool UGridInventoryWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	// 상단 25 왼쪽 20 
	if (InOperation)
	{
		USlotDragDropOperation* Ddop = Cast<USlotDragDropOperation>(InOperation);
		if (IsValid(Ddop))
		{
			FVector2D LocalPos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
			if (OFFSET_Y <= LocalPos.Y && LocalPos.Y <= (OFFSET_Y + SLOTSIZE_Y * INVENTORY_GRID_Y) &&
				OFFSET_X <= LocalPos.X && LocalPos.X <= (OFFSET_X + SLOTSIZE_X * INVENTORY_GRID_X))
			{
				int Y = FMath::Floor(LocalPos.Y - OFFSET_Y);
				int X = FMath::Floor(LocalPos.X - OFFSET_X);

				Y /= SLOTSIZE_Y;
				X /= SLOTSIZE_X;

				FString Msg = FString::Printf(TEXT("%d , %d         %s"), Y, X, *LocalPos.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, *Msg);

				int32 TargetIdx = Y * INVENTORY_GRID_X + X;

				if (0 <= TargetIdx && TargetIdx < INVENTORY_GRID_X * INVENTORY_GRID_Y)
				{
					// 장비나 퀵슬롯은 인덱스를 인벤토리의 인덱스를 가지지 않으

					bool bCanMove = false;
					if (false == Ddop->mSlotTag.MatchesTag(TAG_ITEM_INVENTORY))
					{
						bCanMove = RequestCanMoveByIdx(TargetIdx, Ddop->mGridSize);
					}

					else if (0 <= Ddop->mIdx && Ddop->mIdx < INVENTORY_GRID_X * INVENTORY_GRID_Y)
					{
						bCanMove = RequestCanMoveItem(Ddop->mIdx, TargetIdx);
					}

					if (IsValid(Highlight))
					{
						Highlight->Activate(true, bCanMove, FIntPoint(Ddop->mGridSize.X * SLOTSIZE_X, Ddop->mGridSize.Y * SLOTSIZE_Y));

						if (CSL_Highlight.IsValid())
						{
							CSL_Highlight->SetPosition(FVector2D(X * SLOTSIZE_X, Y * SLOTSIZE_Y));
						}
					}
					return true;
				}
				else
				{
					return true;
				}
			}
			else
			{
				if (IsValid(Highlight))
				{
					Highlight->Activate(false, false);
				}
			}
		}
	}

	return true;
}

bool UGridInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotDragDropOperation* Ddop = Cast<USlotDragDropOperation>(InOperation);
	if (IsValid(Ddop))
	{
		FVector2D LocalPos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		if (OFFSET_Y <= LocalPos.Y && LocalPos.Y <= (OFFSET_Y + SLOTSIZE_Y * INVENTORY_GRID_Y) &&
			OFFSET_X <= LocalPos.X && LocalPos.X <= (OFFSET_X + SLOTSIZE_X * INVENTORY_GRID_X))
		{
			int Y = FMath::Floor(LocalPos.Y - OFFSET_Y);
			int X = FMath::Floor(LocalPos.X - OFFSET_X);

			Y /= SLOTSIZE_Y;
			X /= SLOTSIZE_X;

			FString Msg = FString::Printf(TEXT("%d , %d         %s"), Y, X, *LocalPos.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, *Msg);

			if (IsValid(Highlight))
			{
				Highlight->Activate(false, false);
			}

			int32 TargetIdx = Y * INVENTORY_GRID_X + X;
			if (0 <= TargetIdx && TargetIdx < INVENTORY_GRID_X * INVENTORY_GRID_Y /*&&
				0 <= Ddop->mIdx && Ddop->mIdx < INVENTORY_GRID_X * INVENTORY_GRID_Y*/)
			{
				// 장비 슬롯 or 퀵 슬롯으로부터 드랍 되면, 해당 아이템은 추가를 요청하게 됨
				if (false == Ddop->mSlotTag.MatchesTag(TAG_ITEM_INVENTORY))
				{
					if (RequestAddItemByIdx(Ddop->mKeyName, TargetIdx, Ddop->mAmount))
					{
						if (Ddop->UnEquip)
						{
							Ddop->UnEquip();
						}
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					// 인벤토리 내부의 아이템끼리 
					if (IsValid(mInvnetorySlotArr[Ddop->mIdx]))
					{
						mInvnetorySlotArr[Ddop->mIdx]->SetVisibility(ESlateVisibility::Visible);
					}
					return RequestMoveItem(Ddop->mIdx, TargetIdx);
				}
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void UGridInventoryWidget::AddNewItem(const FSlotInfo& NewItemInfo)
{
	if (NewItemInfo.KeyName.IsNone() ||  false == IsValid(CV_ItemOverlay))
	{
		return;
	}

	if (IsValid(mItemSlotOverlayClass))
	{
		UInventorySlotWidget* SlOverlayWidget = CreateWidget<UInventorySlotWidget>(this, mItemSlotOverlayClass);
		if (IsValid(SlOverlayWidget))
		{	
			int Height = NewItemInfo.GridSize.Y* SLOTSIZE_Y;
			int Width  = NewItemInfo.GridSize.X* SLOTSIZE_X;
			mInvnetorySlotArr[NewItemInfo.Idx] = SlOverlayWidget;

			SlOverlayWidget->Init(NewItemInfo, this);
			auto SL = CV_ItemOverlay->AddChildToCanvas(SlOverlayWidget);
			if (IsValid(SL))
			{
			
				int32 Col = NewItemInfo.Idx % INVENTORY_GRID_X;
				int32 Row = NewItemInfo.Idx / INVENTORY_GRID_X;

				int PositionOffsetX = 0; //(0.5 + 2 * 0.5 * Col);
				int PositionOffsetY = 0; //(0.5 + 2 * 0.5 * Row);
				int SizeOffsetX		= 0; //((NewItemInfo.GridSize.X - 1) * 1);
				int SizeOffsetY		= 0; //((NewItemInfo.GridSize.Y - 1) * 1);

				SL->SetAnchors(FAnchors(0, 0));
				SL->SetPosition(FVector2d(PositionOffsetX + Col * SLOTSIZE_X, PositionOffsetY + Row * SLOTSIZE_Y));
				SL->SetAutoSize(true);
			}
		}
	}

	// 인벤토리의 아이템 소유 목록 배열에 변동이 발생했을때 호출되는 함수로 
	// 해당 패널의 정보를 변경시켜준다 
}

void UGridInventoryWidget::ChangedItem(const FSlotInfo& NewItemInfo)
{
	int32 ChangedSlotIdx = NewItemInfo.Idx;

	if (IsValid(mInvnetorySlotArr[ChangedSlotIdx]))
	{
		mInvnetorySlotArr[ChangedSlotIdx]->Init(NewItemInfo, this);
		mInvnetorySlotArr[ChangedSlotIdx]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGridInventoryWidget::SwapedItem(const FIntPoint& MovedIdx)
{
	if (mInvnetorySlotArr.IsEmpty())
	{
		return;
	}

	//MovedX / MovedY
	//Target / Current 

	// 여기가 제대로 안그려짐 

	int32 CurrentIdx = MovedIdx.X;
	int32 TargetIdx = MovedIdx.Y;

	auto CurrentSlot = mInvnetorySlotArr[CurrentIdx];
	if (IsValid(CurrentSlot))
	{
		auto SL = CurrentSlot->Slot;
		if (SL)
		{
			CurrentSlot->SetIdx(MovedIdx.Y);
			auto CSL = Cast<UCanvasPanelSlot>(SL);
			if (IsValid(CSL))
			{
				int32 Col = TargetIdx % INVENTORY_GRID_X;
				int32 Row = TargetIdx / INVENTORY_GRID_X;
				CSL->SetPosition(FVector2d(Col * SLOTSIZE_X, Row * SLOTSIZE_Y));
			}
		}
	}

	auto TargetSlot = mInvnetorySlotArr[TargetIdx];
	if (IsValid(TargetSlot))
	{
		auto SL = TargetSlot->Slot;
		if (SL)
		{
			TargetSlot->SetIdx(MovedIdx.X);
			auto CSL = Cast<UCanvasPanelSlot>(SL);
			if (IsValid(CSL))
			{
				int32 Col = CurrentIdx % INVENTORY_GRID_X;
				int32 Row = CurrentIdx / INVENTORY_GRID_X;
				CSL->SetPosition(FVector2d(Col * SLOTSIZE_X, Row * SLOTSIZE_Y));
			}
		}
	}
	
	mInvnetorySlotArr.SwapMemory(MovedIdx.Y, MovedIdx.X);
}


//이동한 다음 재이동이 안되고, Swap 이 안되는 상태로 오늘 마무리

void UGridInventoryWidget::MovedItem(const FIntPoint& MovedIdx)
{
	if (mInvnetorySlotArr.IsEmpty())
	{
		return;
	}

	int32 CurrentIdx = MovedIdx.X;
	int32 TargetIdx = MovedIdx.Y;
	
	auto CurrentSlot = mInvnetorySlotArr[CurrentIdx];
	if (IsValid(CurrentSlot))
	{
		auto SL = CurrentSlot->Slot;
		if (SL)
		{
			CurrentSlot->SetIdx(MovedIdx.Y);
			auto CSL = Cast<UCanvasPanelSlot>(SL);
			if (IsValid(CSL))
			{
				int32 Col = TargetIdx % INVENTORY_GRID_X;
				int32 Row = TargetIdx / INVENTORY_GRID_X;
				CSL->SetPosition(FVector2d(Col * SLOTSIZE_X, Row * SLOTSIZE_Y));
			}
		}
		mInvnetorySlotArr.SwapMemory(CurrentIdx, TargetIdx);
	}

}

void UGridInventoryWidget::RemovedItem(const int32& RemovedIdx)
{
	if (IsValid(mInvnetorySlotArr[RemovedIdx]))
	{
		mInvnetorySlotArr[RemovedIdx]->RemoveFromParent();
		mInvnetorySlotArr.RemoveAtSwap(RemovedIdx, EAllowShrinking::No);
	}
}

void UHighlightWidget::Activate(bool bUse, bool bCanMove, const FIntPoint& Size)
{
	if (bUse)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (bCanMove)
	{
		Img_Highlight->SetColorAndOpacity(Green);
	}
	else
	{
		Img_Highlight->SetColorAndOpacity(Red);
	}

	if (IsValid(SB_Size))
	{
		SB_Size->SetWidthOverride(Size.X);
		SB_Size->SetHeightOverride(Size.Y);
	}
}

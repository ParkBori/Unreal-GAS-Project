// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/BaseSlotWidget.h"
#include "UI/Inventory/GridInventoryWidget.h"
#include "Tag/GameplayTag.h"



void UBaseSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	//UE_LOG(LogTemp, Display, TEXT("슬롯에 마우스가 들어가면, 툴팁을 띄워줄거에요"));

	ToggleToolTip(true);
}

void UBaseSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) 
{
	Super::NativeOnMouseLeave(InMouseEvent);
	//UE_LOG(LogTemp, Display, TEXT("슬롯에 마우스가 들어가면, 툴팁을 제거할거에요"));

	ToggleToolTip(false);
}

FReply UBaseSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 프리뷰 위젯 생성 
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 1. 마우스 왼쪽 버튼 클릭인지 확인
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// 2. FReply 구조체의 DetectDrag 함수를 사용하여 드래그 감지 시작을 요청합니다.
		// 이 함수는 Mouse Down 이벤트가 발생한 위젯 자신을 반환하여 드래그 오퍼레이션의 소스로 지정합니다.
		return FReply::Handled().DetectDrag(
			// 이 위젯 (슬롯) 자신을 드래그 소스로 지정
			TakeWidget(), 
			// 감지할 마우스 버튼 키
			EKeys::LeftMouseButton
		);
	}

	// 다른 마우스 버튼은 처리하지 않고 엔진으로 돌려보냄
	return FReply::Unhandled();
}

FReply UBaseSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 프리뷰 위젯 생성 
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	SetSlotVisibility(true);
	return Reply;
}


void UBaseSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	// 드래그 드랍이 감지시 호출 
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Error, TEXT("NativeOnDragDetected"));

	RequestUnHoveredEvent();


	// 1. 드래그 정보 생성(드래그시 담을 데이터) 
	USlotDragDropOperation* DragDropOp = NewObject<USlotDragDropOperation>(this);
	if (nullptr == DragDropOp)
	{
		return;
	}
	
	// 2. 데이터 설정 
	DragDropOp->mKeyName = mItemSlotInfo.KeyName;
	DragDropOp->mIdx = mItemSlotInfo.Idx;
	DragDropOp->mAmount = mItemSlotInfo.Amount;
	DragDropOp->mGridSize = mItemSlotInfo.GridSize;
	DragDropOp->mItemTag = mItemSlotInfo.ItemTag;
	DragDropOp->mSlotTag = mSlotTag;
	//DragDropOp->RemoveItem = [this](int32 id) { if (IsValid(mOwner)) mOwner->RequestRemoveItem(id); };
	// 3. 드래그 때 보여줄 위젯 
	if (nullptr == mDragWidgetClass)
	{
		return;
	}

	UDragWidget* DragWidget = CreateWidget<UDragWidget>(this, mDragWidgetClass);
	if (nullptr == DragWidget)
	{
		return;
	}

	DragWidget->SB_Size->SetWidthOverride(mItemSlotInfo.GridSize.X * SLOTSIZE_X);
	DragWidget->SB_Size->SetHeightOverride(mItemSlotInfo.GridSize.Y * SLOTSIZE_Y);
	DragWidget->Img_Icon->SetBrushFromTexture(mItemSlotInfo.Icon);

	// 4. 드래그 정보ㅔ 해당 위젯을 설정 
 	DragDropOp->DefaultDragVisual = DragWidget;
	// 5. 생성된 오퍼레이션을 반환
	OutOperation = DragDropOp;

	// 6. 원래 슬롯의 아이템 위젯을 숨김 처리 (옵션)
	SetSlotVisibility(false);
}

void UBaseSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	SetSlotVisibility(true);
}

void UBaseSlotWidget::ToggleToolTip(bool bEnable)
{
	if (bEnable)
	{
		// 기존능력치 + 다른 능력치까지 변화 받기
		if (false == mItemSlotInfo.KeyName.IsNone())
		{
			const FGeometry& SlotGeo = GetCachedGeometry();
			const FVector2D& SlotSize = SlotGeo.GetLocalSize();
			FVector2D AbsolyutePosition = SlotGeo.GetAbsolutePosition();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *SlotSize.ToString());

			// 툴팁 
			RequestUpdateToolTip(mItemSlotInfo.KeyName, mSlotTag, AbsolyutePosition, SlotSize);
		}
	}
	else
	{
		RequestUnHoveredEvent();
	}
}

void UBaseSlotWidget::SetSlotVisibility(bool bVisible)
{
	ESlateVisibility NewVisibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	if (mSlotTag.MatchesTag(TAG_ITEM_INVENTORY))
	{
		SetVisibility(NewVisibility);
	}
	else
	{
		Img_Icon->SetVisibility(NewVisibility);
	}
}


void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	USlotDragDropOperation* Ddop = Cast<USlotDragDropOperation>(OutOperation);
	if (IsValid(Ddop))
	{
		Ddop->RemoveItem = [this](int32 Idx) { if (IsValid(mOwner)) mOwner->RequestRemoveItem(Idx); };
		Ddop->ChangeItem = [this](int32 Idx, const FName& ItemName, const FGameplayTag& SlotTag) { if (IsValid(mOwner)) mOwner->RequestChangeItem(Idx, ItemName, SlotTag); };
	}
}

void UInventorySlotWidget::Init(const FSlotInfo& ItemSlotInfo, UGridInventoryWidget* Owner)
{
	mItemSlotInfo = ItemSlotInfo;

	if (IsValid(SB_Size))
	{
		SB_Size->SetWidthOverride(mItemSlotInfo.GridSize.X * SLOTSIZE_X);
		SB_Size->SetHeightOverride(mItemSlotInfo.GridSize.Y * SLOTSIZE_Y);
	}

	if (IsValid(Img_Icon) && IsValid(mItemSlotInfo.Icon))
	{
		Img_Icon->SetBrushFromTexture(mItemSlotInfo.Icon);
	}

	if (false == mItemSlotInfo.ItemTag.MatchesTag(TAG_ITEM_EQUIPMENT))
	{
		TB_Amount->SetText(FText::AsNumber(mItemSlotInfo.Amount));
		TB_Amount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	if (IsValid(Owner))
	{
		mOwner = Owner;
	}

}




void UInventorySlotWidget::SetIdx(int32 NewIdx)
{
	mItemSlotInfo.Idx = NewIdx;
}

bool UEquipmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotDragDropOperation* Ddop = Cast<USlotDragDropOperation>(InOperation);
	if (IsValid(Ddop))
	{
		if (Ddop->mItemTag.MatchesTagExact(mItemSlotInfo.ItemTag))
		{
			SetSlotVisibility(true);
			return true;
		}

		if (Ddop->mItemTag.MatchesTag(mSlotTag))
		{
			// 슬롯이 비어있지 않는 경우
			if (mItemSlotInfo.ItemTag.IsValid())
			{
				Ddop->ChangeItem(Ddop->mIdx, Ddop->mKeyName, mSlotTag);
				// 드랍으로 들어온 아이템의 인데스에, 현재 장비슬롯의 아이템이 들어간다
				return true;
			}
			// 슬롯이 비어있는 경우
			else
			{
				// 장비슬롯에 아이템 세팅 
				RequestEquip(Ddop->mKeyName, mSlotTag);
				if (Ddop->RemoveItem)
				{
					Ddop->RemoveItem(Ddop->mIdx);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UEquipmentSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	USlotDragDropOperation* Ddop = Cast<USlotDragDropOperation>(OutOperation);
	if (IsValid(Ddop))
	{
		Ddop->UnEquip = [this]() { RequestUnEquip(mSlotTag); };
	}
}

void UEquipmentSlotWidget::Callback_EquipItem(const FSlotInfo& NewEquipedItemInfo)
{
	mItemSlotInfo = NewEquipedItemInfo;

	if (IsValid(Img_Icon) && nullptr != mItemSlotInfo.Icon)
	{
		Img_Icon->SetBrushFromTexture(NewEquipedItemInfo.Icon);
		SetSlotVisibility(true);
	}
}

void UEquipmentSlotWidget::Callback_UnEquipItem(bool bValue)
{
	mItemSlotInfo.ItemTag = FGameplayTag::EmptyTag;
	SetSlotVisibility(false);
}

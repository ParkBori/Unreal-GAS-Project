// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemSlotWidget.h"
#include "Shared/Inventory/Snapshot.h"
#include "Subsystem/ItemSubsystem.h"


// 전체적인 슬롯 구조가, 직접적인 의존성이 아닌, ItemSubsystem 바탕으로 작동을 시켜야 한다.


void UItemSlotWidget::Update(const FInventoryUISnapshot& Snapshot)
{
	bUsing = !Snapshot.bEmpty;

	if (bUsing)
	{
		if (Snapshot.CurrentAmount > 1)
		{
			TB_ItemAmount->SetVisibility(ESlateVisibility::HitTestInvisible);
			TB_ItemAmount->SetText(FText::AsNumber(Snapshot.CurrentAmount));
		}
		else
		{
			TB_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (Snapshot.Icon)
		{
			Img_ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			Img_ItemIcon->SetBrushFromTexture(Snapshot.Icon);
		}
	}
	else
	{
		Img_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		TB_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	auto* IS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (IS)
	{
		// 툴팀 관련 
		IS->RequestActivateSlotItemInteract(GetOwningPlayerPawn(), mSlotContext, InGeometry);
	}
}

void UItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	auto* IS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (IS)
	{
		// 툴팀 관련 
		IS->RequestDeActivateSlotItemInteract(GetOwningPlayerPawn(), mSlotContext);
	}
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 프리뷰 위젯 생성 
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	// 사용되고 있는 슬롯만이 작동을 해야함

	// Img_ItemIcon 은 위젯을 가리키고 있으니깐 항상 true 로, 모든 상황에서 드래그가 허용됨

	if (IsUsing())
	{
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
		if (ESlotType::Quick == mSlotContext.SlotType
			&& InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			
			auto* IS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
			if (IS)
			{
				// 툴팀 관련 
				IS->RequestClearQuickSlot(GetOwningPlayerPawn(), mSlotContext.SlotIndex);
			}
		}
	}

	// 다른 마우스 버튼은 처리하지 않고 엔진으로 돌려보냄
	return FReply::Unhandled();
}

FReply UItemSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	return Reply;
}


void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	// 드래그 드랍이 감지시 호출 
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 1. 드래그 정보 생성(드래그시 담을 데이터) 
	UItemSlotDragDropOperation* DragDropOp = NewObject<UItemSlotDragDropOperation>(this);
	if (nullptr == DragDropOp)
	{
		return;
	}

	// 2. 데이터 설정 
	DragDropOp->mSlotContext = mSlotContext;

	// 3. 드래그 때 보여줄 위젯 
	if (nullptr == mDragWidgetClass)
	{
		return;
	}

	UItemSlotDragWidget* DragWidget = CreateWidget<UItemSlotDragWidget>(this, mDragWidgetClass);
	if (nullptr == DragWidget)
	{
		return;
	}
	
	DragWidget->Img_Icon->SetBrush(Img_ItemIcon->GetBrush());

	// 4. 드래그 정보ㅔ 해당 위젯을 설정 
	DragDropOp->DefaultDragVisual = DragWidget;

	// 5. 생성된 오퍼레이션을 반환
	OutOperation = DragDropOp;

	// 6. 원래 슬롯의 아이템 위젯을 숨김 처리 (옵션)
	//Img_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}


bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	MY_LOG(Warning, "");

	UItemSlotDragDropOperation* DragDropOperation = Cast<UItemSlotDragDropOperation>(InOperation);
	if (IsValid(DragDropOperation))
	{
		const FSlotContext& CurrentSlotContext = DragDropOperation->mSlotContext;
		const FSlotContext& TargetSlotContext = mSlotContext;

		auto* IS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
		if (IS)
		{
			IS->RequestItemMove(GetOwningPlayerPawn(), CurrentSlotContext, TargetSlotContext);
		}
	}

	return true;
}
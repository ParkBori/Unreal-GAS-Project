// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUserWidget.h"
#include "UIManager.h"
#include "Slate/SObjectWidget.h"
#include "Widgets/SWidget.h"

UBaseUserWidget::UBaseUserWidget(const FObjectInitializer& ObjectInitializer)	:
	UUserWidget(ObjectInitializer)
{
}

void UBaseUserWidget::PlayWidgetAnimation(const FString& Name,
	float PlayStartTime, float PlaySpeed, bool Forward, 
	bool RestoreState, int32 LoopCount)
{
	TObjectPtr<UWidgetAnimation>	Anim = mAnimMap.FindRef(Name);

	if (IsValid(Anim))
	{
		EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward;

		if (!Forward)
			PlayMode = EUMGSequencePlayMode::Reverse;

		PlayAnimation(Anim, PlayStartTime, LoopCount, PlayMode,
			PlaySpeed, RestoreState);
	}
}

// �ʱ�ȭ 1.
void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	////UE_LOG(SacGame, Warning, TEXT("NativeOnInitialized : %s"),
	//	*mWidgetName);

	CUIManager::GetInst()->AddWidget(mWidgetName, this);
}

// �ʱ�ȭ 2.
void UBaseUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	////UE_LOG(SacGame, Warning, TEXT("NativePreConstruct"));
}

// �ʱ�ȭ 3.
void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	////UE_LOG(SacGame, Warning, TEXT("NativeConstruct"));

	// ������ ������ �ִ� �ִϸ��̼��� �����ͼ� Map�� �����صд�.
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	//int32	AnimCount = WidgetClass->Animations.Num();

	for (auto& Anim : WidgetClass->Animations)
	{
		// �ִϸ��̼� �̸��� ���´�.
		FString	Name = Anim->GetName();

		// �ִϸ��̼� �̸��� �ڿ� _INST�� �پ���� ������ �����Ѵ�.
		Name.ReplaceInline(TEXT("_INST"), TEXT(""), ESearchCase::IgnoreCase);

		mAnimMap.Add(Name, Anim);
	}
}

// ����� �� ���´�.
void UBaseUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	////UE_LOG(SacGame, Warning, TEXT("NativeDestruct"));

	CUIManager::GetInst()->DeleteWidget(mWidgetName);
}

void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// �Ź� �׷��ٶ� ���´�.
int32 UBaseUserWidget::NativePaint(const FPaintArgs& Args, 
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	int32 result = Super::NativePaint(Args, AllottedGeometry, 
		MyCullingRect, OutDrawElements,
		LayerId, InWidgetStyle, bParentEnabled);

	////UE_LOG(SacGame, Warning, TEXT("NativePaint"));

	return result;
}

FReply UBaseUserWidget::NativeOnFocusReceived(const FGeometry& InGeometry,
	const FFocusEvent& InFocusEvent)
{
	FReply result = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

	////UE_LOG(SacGame, Warning, TEXT("NativeOnFocusReceived"));

	return result;
}

void UBaseUserWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	////UE_LOG(SacGame, Warning, TEXT("NativeOnFocusLost"));
}

void UBaseUserWidget::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath,
	const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath,
		InFocusEvent);

	////UE_LOG(SacGame, Warning, TEXT("NativeOnFocusChanging"));
}

void UBaseUserWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnAddedToFocusPath"));
}

void UBaseUserWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnRemovedFromFocusPath"));
}

FNavigationReply UBaseUserWidget::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent,
	const FNavigationReply& InDefaultReply)
{
	FNavigationReply result = Super::NativeOnNavigation(MyGeometry,
		InNavigationEvent, InDefaultReply);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnNavigation"));

	return result;
}

FReply UBaseUserWidget::NativeOnKeyChar(const FGeometry& InGeometry,
	const FCharacterEvent& InCharEvent)
{
	FReply result = Super::NativeOnKeyChar(InGeometry,
		InCharEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnKeyChar"));

	return result;
}

FReply UBaseUserWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry,
	const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnPreviewKeyDown(InGeometry,
		InKeyEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnPreviewKeyDown"));

	return result;
}

FReply UBaseUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, 
	const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry,
		InKeyEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnKeyDown"));

	return result;
}

FReply UBaseUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, 
	const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyUp(InGeometry,
		InKeyEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnKeyUp"));

	return result;
}

FReply UBaseUserWidget::NativeOnAnalogValueChanged(const FGeometry& InGeometry,
	const FAnalogInputEvent& InAnalogEvent)
{
	FReply result = Super::NativeOnAnalogValueChanged(InGeometry,
		InAnalogEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnAnalogValueChanged"));

	return result;
}

FReply UBaseUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry,
		InMouseEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseButtonDown"));

	return result;
}

FReply UBaseUserWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnPreviewMouseButtonDown(InGeometry,
		InMouseEvent);

	if (mParentWidget)
	{
		UBaseUserWidget* ParentWidget =
			Cast<UBaseUserWidget>(mParentWidget);

		if (ParentWidget)
		{
			ParentWidget->SetFocusWidget(this);
		}
	}

	return result;
}

FReply UBaseUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, 
	const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonUp(InGeometry,
		InMouseEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseButtonUp"));

	return result;
}

FReply UBaseUserWidget::NativeOnMouseMove(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseMove(InGeometry,
		InMouseEvent);

	////UE_LOG(SacGame, Warning, TEXT("NativeOnMouseMove"));

	return result;
}

void UBaseUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseEnter"));
}

void UBaseUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseLeave"));
}

FReply UBaseUserWidget::NativeOnMouseWheel(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseWheel(InGeometry,
		InMouseEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseWheel"));

	return result;
}

FReply UBaseUserWidget::NativeOnMouseButtonDoubleClick(
	const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDoubleClick(InGeometry,
		InMouseEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseButtonDoubleClick"));

	return result;
}

void UBaseUserWidget::NativeOnDragDetected(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnDragDetected"));
}

void UBaseUserWidget::NativeOnDragEnter(const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnDragEnter"));
}

void UBaseUserWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnDragLeave"));
}

bool UBaseUserWidget::NativeOnDragOver(const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDragOver(InGeometry, InDragDropEvent,
		InOperation);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnDragOver"));

	return result;
}

bool UBaseUserWidget::NativeOnDrop(const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDrop(InGeometry, InDragDropEvent,
		InOperation);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnDrop"));

	return result;
}

void UBaseUserWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnDragCancelled"));
}

FReply UBaseUserWidget::NativeOnTouchGesture(const FGeometry& InGeometry,
	const FPointerEvent& InGestureEvent)
{
	FReply result = Super::NativeOnTouchGesture(InGeometry,
		InGestureEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnTouchGesture"));

	return result;
}

FReply UBaseUserWidget::NativeOnTouchStarted(const FGeometry& InGeometry
	, const FPointerEvent& InGestureEvent)
{
	FReply result = Super::NativeOnTouchStarted(InGeometry,
		InGestureEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnTouchStarted"));

	return result;
}

FReply UBaseUserWidget::NativeOnTouchMoved(const FGeometry& InGeometry,
	const FPointerEvent& InGestureEvent)
{
	FReply result = Super::NativeOnTouchMoved(InGeometry,
		InGestureEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnTouchMoved"));

	return result;
}

FReply UBaseUserWidget::NativeOnTouchEnded(const FGeometry& InGeometry,
	const FPointerEvent& InGestureEvent)
{
	FReply result = Super::NativeOnTouchEnded(InGeometry,
		InGestureEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnTouchEnded"));

	return result;
}

FReply UBaseUserWidget::NativeOnMotionDetected(const FGeometry& InGeometry,
	const FMotionEvent& InMotionEvent)
{
	FReply result = Super::NativeOnMotionDetected(InGeometry,
		InMotionEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMotionDetected"));

	return result;
}

FReply UBaseUserWidget::NativeOnTouchForceChanged(const FGeometry& MyGeometry,
	const FPointerEvent& TouchEvent)
{
	FReply result = Super::NativeOnTouchForceChanged(MyGeometry,
		TouchEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnTouchForceChanged"));

	return result;
}

FCursorReply UBaseUserWidget::NativeOnCursorQuery(const FGeometry& InGeometry,
	const FPointerEvent& InCursorEvent)
{
	FCursorReply result = Super::NativeOnCursorQuery(InGeometry,
		InCursorEvent);

	////UE_LOG(SacGame, Warning, TEXT("NativeOnCursorQuery"));

	return result;
}

FNavigationReply UBaseUserWidget::NativeOnNavigation(const FGeometry& InGeometry,
	const FNavigationEvent& InNavigationEvent)
{
	FNavigationReply result = Super::NativeOnNavigation(InGeometry,
		InNavigationEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnNavigation"));

	return result;
}

void UBaseUserWidget::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);

	//UE_LOG(SacGame, Warning, TEXT("NativeOnMouseCaptureLost"));
}


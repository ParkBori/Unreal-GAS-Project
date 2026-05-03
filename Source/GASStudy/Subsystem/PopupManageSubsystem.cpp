// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PopupManageSubsystem.h"

#include "Kismet/GameplayStatics.h"

UPopupManageSubsystem::UPopupManageSubsystem()
{
	static ConstructorHelpers::FClassFinder<UPopupWidget> PopupWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Popup/WBP_Popup.WBP_Popup_C'"));
	if (PopupWidgetAsset.Succeeded())
	{
		mPopupWidgetClass = PopupWidgetAsset.Class;
	}

	static ConstructorHelpers::FClassFinder<UPopupWidget> TimerPopupWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Popup/WBP_PopupTimer.WBP_PopupTimer_C'"));
	if (PopupWidgetAsset.Succeeded())
	{
		mTimerPopupWidgetClass = TimerPopupWidgetAsset.Class;
	}
}

void UPopupManageSubsystem::PopupClosed()
{
	bIsPopupOpened = false;

	// 큐가 비어있지 않다면, 큐에 존재하는 정보를 바탕으로 팝업을 생성한다.
	if (false == mPopupQue.IsEmpty())
	{
		CreatePopupWidget();
	}
}

bool UPopupManageSubsystem::CreatePopup(const FPopupInfo& PopupInfo)
{
	// 들어오면 일단 넣음(큐가 항상 채워진 상태로 로직에 들어간다)
	mPopupQue.Enqueue(PopupInfo);

	// 열려있으면 생성하지 않고 큐에 저장
	if (bIsPopupOpened)
	{
		return false;
	}
	// 열려있지 않다면, 큐에서 빼서 사용
	else
	{
		CreatePopupWidget();
		return true;
	}
}

bool UPopupManageSubsystem::CreatePopupFactory(EPopupType PopupType)
{
	// 들어오면 일단 넣음(큐가 항상 채워진 상태로 로직에 들어간다)
	FPopupInfo PopupInfo = CPopupFactory::CreatePopup(PopupType);

	mPopupQue.Enqueue(PopupInfo);

	// 열려있으면 생성하지 않고 큐에 저장
	if (bIsPopupOpened)
	{
		return false;
	}
	// 열려있지 않다면, 큐에서 빼서 사용
	else
	{
		CreatePopupWidget();
		return true;
	}
}

void UPopupManageSubsystem::CreatePopupWidget()
{
	FPopupInfo QueFrontInfo;
	mPopupQue.Dequeue(QueFrontInfo);

	UPopupWidget* Popup = nullptr;
	if (!QueFrontInfo.bUserTimer)
	{
		Popup = CreateWidget<UPopupWidget>(GetWorld()->GetFirstPlayerController(), mPopupWidgetClass);
	}
	else
	{
		UTimerPopupWidget* TimerPopup = CreateWidget<UTimerPopupWidget>(GetWorld()->GetFirstPlayerController(), mTimerPopupWidgetClass);
		if (IsValid(TimerPopup))
		{
			TimerPopup->SetRemainingTime(QueFrontInfo.RemainingTime);
			Popup = TimerPopup;
		}
	}

	if (IsValid(Popup))
	{
		Popup->Init(QueFrontInfo.Title, QueFrontInfo.Script, QueFrontInfo.OnConfirmDelegate, QueFrontInfo.OnCancleDelegate, QueFrontInfo.bOnlyConfirm);
		Popup->mOnConfirm.AddDynamic(this, &UPopupManageSubsystem::Callback_ClosedPopup);
		Popup->mOnCancle.AddDynamic(this, &UPopupManageSubsystem::Callback_ClosedPopup);
		Popup->mOnConfirm.AddDynamic(this, &UPopupManageSubsystem::ClearFocusPopup);
		Popup->mOnCancle.AddDynamic(this, &UPopupManageSubsystem::ClearFocusPopup);
		FocusPopup(Popup);
		Popup->AddToViewport(99);
		bIsPopupOpened = true;
	}
}

void UPopupManageSubsystem::Callback_ClosedPopup()
{
	bIsPopupOpened = false;
}

void UPopupManageSubsystem::FocusPopup(UUserWidget* PopupWidget)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	UGameplayStatics::SetGamePaused(GetWorld(), true);


	bCacheShowMouseCursor = PC->bShowMouseCursor;

	PC->bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(PopupWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
}

void UPopupManageSubsystem::ClearFocusPopup()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	PC->bShowMouseCursor = bCacheShowMouseCursor;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
}


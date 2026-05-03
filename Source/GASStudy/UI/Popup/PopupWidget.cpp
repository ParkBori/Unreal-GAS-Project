// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/PopupWidget.h"
#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"
#include "Subsystem/PopupManageSubsystem.h"
#include  "Kismet/KismetSystemLibrary.h"

void UPopupWidget::NativeDestruct()
{
	/**
	* 해당 로직은 꼭 필요한데.. 이게 NativeDestruct 랑 파업 버튼 콜백이랑 고민을 할 수 있다.
	* 여기에 두면 살짝 위험해보이는게, RemoveFromParent 를 한다고해서, 항상 즉시 파괴된다고
	* 100% 장담할수 없다. 현재는 그러한 코드가 없지만, 만약 다른쪽에서 팝업을 물고있으면
	* 이때는 바로 파괴가 되지 않기 때문..!
	* 
	* 하지만 여기에 작성하면 코드가 더 깔끔해지긴 한다. 여기에 두도록하고, 문제가 생기면 옮기는 걸로..
	*/
	UPopupManageSubsystem* PopupMangeSubSystem = GetGameInstance()->GetSubsystem<UPopupManageSubsystem>();
	if (IsValid(PopupMangeSubSystem))
	{
		PopupMangeSubSystem->PopupClosed();
	}

	Super::NativeDestruct();
}

void UPopupWidget::Init(FText Title, FText Script, FOnConfirm OnConfirmDelegate, FOnCancle OnCancleDelegate, bool bOnlyConfirm)
{
	// 타이틀 정보
	if (IsValid(CT_Title))
	{
		CT_Title->SetText(Title);
	}

	// 내용 
	if (IsValid(CT_Script))
	{
		CT_Script->SetText(Script);
	}

	CreateAndAddButton(FText::FromString(TEXT("Confirm")), [&]() { OnClickedCallback_Confirm();  });
	
	if (!bOnlyConfirm)
	{
		CreateAndAddButton(FText::FromString(TEXT("Cancle")), [&]() { OnClickedCallback_Cancle(); });
	}

	mOnConfirm = OnConfirmDelegate;
	mOnCancle = OnCancleDelegate;

}

void UPopupWidget::CreateAndAddButton(FText ButtonName, TFunction<void()> CallFunc)
{
	UBaseButtonWidget* Btn = CreateWidget<UBaseButtonWidget>(this, mButtonClass);
	if (IsValid(Btn))
	{
		Btn->SetBtnName(ButtonName);
		Btn->mTextStyle.Noraml.TextStyle = mNormalStyle;
		Btn->mTextStyle.Hover.TextStyle = mHoverStyle;

		if (IsValid(HB_Buttons))
		{
			UHorizontalBoxSlot* HBSL = HB_Buttons->AddChildToHorizontalBox(Btn);
			if (IsValid(HBSL))
			{
				HBSL->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				HBSL->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				HBSL->SetPadding(FMargin(50.f, 0.f, 50.f, 0.f));
			}
			
			// CallFunc 를 복사로 캡쳐해야함. CallFunc 는 함수지역변수 이기때문에, 해당 함수를 넘어가면 사라진다.
			// 여기서 TFunc 는 포인터로 복사되기때문에 이는 유효하다.
			Btn->OnClicked().AddLambda([this, CallFunc]() {
					CallFunc();
				});

		
		}
	}
}

void UPopupWidget::OnClickedCallback_Confirm()
{
	if (mOnConfirm.IsBound())
	{
		mOnConfirm.Broadcast();
	}

	RemoveFromParent();
}

void UPopupWidget::OnClickedCallback_Cancle()
{
	if (mOnCancle.IsBound())
	{
		mOnCancle.Broadcast();
	}

	RemoveFromParent();
}


void UTimerPopupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	mAcc += InDeltaTime;
	if (mAcc >= 1.f)
	{
		mAcc = 0.0f;
		--mRemainingTime;
		SetText(mRemainingTime);
		
		if (mRemainingTime == 0)
		{
			// 제거
			OnClickedCallback_Cancle();
		}
	}
}

void UTimerPopupWidget::SetText(int32 Seconds)
{
	if (IsValid(CT_Count))
	{
		// 초 -> 시간 분으로 변경
		FIntPoint ConvertedTime = ConvertSecondsToTime(mRemainingTime);

#define LOCTEXT_NAMESPACE "CountText"
		// 기존의 포맷팅을 사용하면 숫자 사이에 , 가 들어가게 된다. 
		// return FText::Format(LOCTEXT("ResolutionText", "{0}x{1}"), Resolution.X, Resolution.Y);
		FNumberFormattingOptions NoGrouping;
		NoGrouping.SetUseGrouping(false);
		CT_Count->SetText(FText::Format(LOCTEXT("CountText", "{0}:{1}"), FText::AsNumber(ConvertedTime.X), FText::AsNumber(ConvertedTime.Y)));
#undef LOCTEXT_NAMESPACE 
	}
}

FIntPoint UTimerPopupWidget::ConvertSecondsToTime(int Seconds)
{
	FIntPoint ConvertedTime;
	ConvertedTime.X = Seconds / 60;
	ConvertedTime.Y = Seconds % 60;

	return ConvertedTime;
}

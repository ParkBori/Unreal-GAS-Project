// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialogue/DialogueHUDWidget.h"
#include "Subsystem/DialogueSubsystem.h"


void UDialogueHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDialogueSubsystem* DS = GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
	if (DS)
	{
		DS->mOnUpdateDialogue.AddUObject(this, &ThisClass::Update);
	}
}

FReply UDialogueHUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 프리뷰 위젯 생성 
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	// 사용되고 있는 슬롯만이 작동을 해야함

	// Img_ItemIcon 은 위젯을 가리키고 있으니깐 항상 true 로, 모든 상황에서 드래그가 허용됨

	if (bCanNextByLeftClick)
	{
		// 1. 마우스 왼쪽 버튼 클릭인지 확인
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			UDialogueSubsystem* DS = GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
			if (DS)
			{
				DS->Next();
			}
		}
	}

	// 다른 마우스 버튼은 처리하지 않고 엔진으로 돌려보냄
	return FReply::Unhandled();
}

void UDialogueHUDWidget::Update(const FUIDialogueData& Data)
{
	RemoveChoice();
	UpdateDialogueText(Data.SpeackerName, Data.DialogueText);
	CreateChoice_V2(Data.Choices);
}

void UDialogueHUDWidget::RemoveChoice()
{
	mChoices.Empty();
	CV_ChocieBox->ClearChildren();
}

void UDialogueHUDWidget::UpdateDialogueText(const FText& Speaker, const FText& Script)
{
	FText Result = FText::Format(
		NSLOCTEXT("Speaker", "Script", "{0} : {1}"),
		Speaker,
		Script
	);

	TB_Dialogue->SetText(Result);
}

void UDialogueHUDWidget::CreateChoice_V2(const TArray<FUIChoiceData>& Choices)
{
	if (!IsValid(CV_ChocieBox))
	{
		return;
	}

	// 기본적으로 클릭으로 넘길 수 있게한다.
	bCanNextByLeftClick = true;
	
	for (const auto& Choice : Choices)
	{
		// 선택지가 있는 경우 넘길 수 없게한다.
		bCanNextByLeftClick = false;

		UDialogueChoiceWidget* DialogueChoiceWidget = CreateWidget<UDialogueChoiceWidget>(this, mDialogueChoiceWidget);
		if (IsValid(DialogueChoiceWidget))
		{
			DialogueChoiceWidget->Init(Choice);
			auto SL = CV_ChocieBox->AddChildToCanvas(DialogueChoiceWidget);
			if (IsValid(SL))
			{
				SL->SetAnchors(FAnchors(0, 1, 0, 1));

				float OffsetY = SL->GetSize().Y * -1;
				SL->SetPosition(FVector2D(10 * (Choice.ChoiceIdx), OffsetY * (Choice.ChoiceIdx)));
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Choice Raw: %s"), *Choice.ChoiceScript.ToString());
	}
}


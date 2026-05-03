// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialogue/DialogueChoiceWidget.h"
#include "Subsystem/DialogueSubsystem.h"


void UDialogueChoiceWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Choice))
	{
		Btn_Choice->OnClicked.AddDynamic(this, &UDialogueChoiceWidget::Btn_Choice_Clicked);
	}
}

void UDialogueChoiceWidget::Init(const FUIChoiceData& Data)
{
	mChoiceData = Data;
	
	if (IsValid(CTB_Script))
	{
		CTB_Script->SetText(Data.ChoiceScript);
		CTB_Number->SetText(FText::AsNumber(Data.ChoiceIdx + 1));
	}

}

void UDialogueChoiceWidget::Btn_Choice_Clicked()
{
	UDialogueSubsystem* DS = GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
	if (DS)
	{
		DS->SelectChoice(mChoiceData.ChoiceIdx);
	}
}

void UDialogueChoiceWidget::ChangeTextStyle(bool bHover)
{
	if (IsValid(CTB_Script) && IsValid(CTB_Number))
	{
		if (bHover)
		{
			if (IsValid(mHoverTextStyle))
			{
				CTB_Script->SetStyle(mHoverTextStyle);
				CTB_Number->SetStyle(mHoverTextStyle);
			}
		}
		else
		{
			if (IsValid(mUnHoverTextStyle))

			{
				CTB_Script->SetStyle(mUnHoverTextStyle);
				CTB_Number->SetStyle(mUnHoverTextStyle);
			}
		}
	}
}


//초이스에서만 한글이 깨지는 문제가 있는데, 일단 분기별로 정상 작동 확인
// -> 로보초가 엔진이 아니라 다른쪽 로보토였음 
// 
//여기에 퀘스트가 연동되는데, 보상, 저장할때, 또한 퀘스트 받은 상태에서 완료시 이어갈수 있게 하는 그런걸 추가해주면 될거같다.
//
//또한 퀘스트 UI 가 너무 안이뻐서 완전 깔끔하게 가는걸로.
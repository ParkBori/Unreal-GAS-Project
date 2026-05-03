// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Credit/CreditWidget.h"
#include "Subsystem/WorldTransitionSubsystem.h"
#include "PlayerController/InGamePlayerController.h"

UCreditWidget::UCreditWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UCreditInfo> DAAsset(TEXT("/Script/GASStudy.CreditInfo'/Game/GASStudy/Data/Credit/DA_Credit.DA_Credit'"));
	if (DAAsset.Succeeded())
	{
		mDA_CreditInfo = DAAsset.Object;
	}
}

void UCreditWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Close))
	{
		Btn_Close->OnClicked.AddDynamic(this, &UCreditWidget::OnClickedCallback_Btn_Close);
	}
}


void UCreditWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(mDA_CreditInfo);

	if (false == IsValid(ScrollBox) || false == IsValid(SB_ScrollBox))
	{
		return;
	}

	{
		USpacer* Spacer = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
		if (IsValid(Spacer))
		{
			Spacer->SetSize(FVector2D(0.f, SB_ScrollBox->GetHeightOverride()));
			ScrollBox->AddChild(Spacer);
		}
	}

	for (const auto& Info : mDA_CreditInfo->CreditMessage)
	{
		FString CreditType;
		switch (Info.Key)
		{
		case ECreditType::ASSET:
			CreditType = TEXT("Asset");
			break;
		case ECreditType::PROGREMER:
			CreditType = TEXT("Programmer");
			break;
		}

		UTextBlock* TBTitlte = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		if (IsValid(TBTitlte))
		{
			TBTitlte->SetText(FText::FromString(CreditType));
			TBTitlte->SetJustification(ETextJustify::Center);

			FSlateFontInfo TBTitleFont = TBTitlte->GetFont();
			TBTitleFont.Size = 36.f;
			TBTitlte->SetFont(TBTitleFont);

			ScrollBox->AddChild(TBTitlte);

			if (UScrollBoxSlot* TBTitleSlot = Cast<UScrollBoxSlot>(TBTitlte->Slot))
			{
				TBTitleSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));  // 아래 여백 8px
			}
		}

		UTextBlock* TBContent = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		if (IsValid(TBContent))
		{
			TBContent->SetText(Info.Value);
			TBContent->SetJustification(ETextJustify::Center);
			ScrollBox->AddChild(TBContent);

			if (UScrollBoxSlot* TBContentSlot = Cast<UScrollBoxSlot>(TBContent->Slot))
			{
				TBContentSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 48.f));  // 아래 여백 16px
			}
		}
	}

	{
		USpacer* Spacer = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
		if (IsValid(Spacer))
		{
			Spacer->SetSize(FVector2D(0.f, SB_ScrollBox->GetHeightOverride() / 2.0f));
			ScrollBox->AddChild(Spacer);
		}
	}

	UTextBlock* TB = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	if (IsValid(TB))
	{
		TB->SetText(FText::FromString(TEXT("Thank you...!")));
		TB->SetJustification(ETextJustify::Center);
		ScrollBox->AddChild(TB);
	}

	{
		USpacer* Spacer = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
		if (IsValid(Spacer))
		{
			Spacer->SetSize(FVector2D(0.f, SB_ScrollBox->GetHeightOverride() / 2.0f));
			ScrollBox->AddChild(Spacer);
		}
	}
}

void UCreditWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(ScrollBox))
	{

		float ScrollEndOffset = ScrollBox->GetScrollOffsetOfEnd();

		if (IsValid(mAlphaCurv))
		{
			float Alpha = mAlphaCurv->GetFloatValue(mAccDelta);
			float CurrentScrollOffset = FMath::Lerp(0, ScrollEndOffset, Alpha);

			ScrollBox->SetScrollOffset(CurrentScrollOffset);

			mAccDelta += InDeltaTime;
			if (FMath::IsNearlyEqual(CurrentScrollOffset, ScrollEndOffset))
			{
				CreditEnd();
			}
		}
	}
}

void UCreditWidget::OnClickedCallback_Btn_Close()
{
	CreditEnd();
}

void UCreditWidget::CreditEnd()
{
	auto PC = GetOwningPlayer<AInGamePlayerController>();
	if (!IsValid(PC))
	{
		return;
	}

	FOnFadeCompleted OnFadeCompleted;
	OnFadeCompleted.BindUFunction(this, TEXT("TransitionMainLevel"));
	PC->StartFade(mWorldTransitionDelay, OnFadeCompleted);
	RemoveFromParent();
}

void UCreditWidget::TransitionMainLevel()
{
	auto PC = GetOwningPlayer();
	if (!IsValid(PC))
	{
		return;
	}

	auto* WT = PC->GetGameInstance()->GetSubsystem<UWorldTransitionSubsystem>();
	if (!WT)
	{
		return;
	}

	FTransitionRequest TR;
	TR.LevelName = TEXT("Main");
	TR.IsNewGame = true;
	WT->RequestTransition(TR);
}





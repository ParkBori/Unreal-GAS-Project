// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/GameMenuWidget.h"
#include "GameMenuButtonWidget.h"

UGameMenuWidget::UGameMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameMenuWidget::NativeOnInitialized()
{
	UGameMenuButtonWidget* Button = Cast<UGameMenuButtonWidget>(GetWidgetFromName(FName(TEXT("EquipButton"))));
	if (IsValid(Button))
	{
		WidgetSwitchButtons.Add(Button);
		Button->OnButtonSelect.BindUFunction(this, TEXT("UpdatePage"));
	}

	Button = Cast<UGameMenuButtonWidget>(GetWidgetFromName(FName(TEXT("InventoryButton"))));
	if (IsValid(Button))
	{
		WidgetSwitchButtons.Add(Button);
		Button->OnButtonSelect.BindUFunction(this, TEXT("UpdatePage"));
	}

	Button = Cast<UGameMenuButtonWidget>(GetWidgetFromName(FName(TEXT("SkillButton"))));
	if (IsValid(Button))
	{
		WidgetSwitchButtons.Add(Button);
		Button->OnButtonSelect.BindUFunction(this, TEXT("UpdatePage"));
	}

	Button = Cast<UGameMenuButtonWidget>(GetWidgetFromName(FName(TEXT("QuestButton"))));
	if (IsValid(Button))
	{
		WidgetSwitchButtons.Add(Button);
		Button->OnButtonSelect.BindUFunction(this, TEXT("UpdatePage"));
	}

	Button = Cast<UGameMenuButtonWidget>(GetWidgetFromName(FName(TEXT("OptionButton"))));
	if (IsValid(Button))
	{
		WidgetSwitchButtons.Add(Button);
		Button->OnButtonSelect.BindUFunction(this, TEXT("UpdatePage"));
	}

	PageSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(FName(TEXT("PageSwitcher"))));
	EquipPage = Cast<UUserWidget>(GetWidgetFromName(FName(TEXT("WBP_EquipPage"))));
	InventoryPage = Cast<UUserWidget>(GetWidgetFromName(FName(TEXT("WBP_InventoryPage"))));
}

void UGameMenuWidget::UpdatePage(EGameMenuType NewType)
{
	if (IsValid(PageSwitcher))
	{
		UUserWidget* NewPage = nullptr;
		switch (NewType)
		{
		case EGameMenuType::EQUIP:
			UE_LOG(LogTemp, Warning, TEXT("Equip"));
			NewPage = EquipPage;
			break;
		case EGameMenuType::INVENTORY:
			UE_LOG(LogTemp, Warning, TEXT("Inventory"));
			NewPage = InventoryPage;
			break;
		case EGameMenuType::SKILL:
			UE_LOG(LogTemp, Warning, TEXT("Skill"));
			break;
		case EGameMenuType::QUEST:
			UE_LOG(LogTemp, Warning, TEXT("Quest"));
			break;
		case EGameMenuType::OPTION:
			UE_LOG(LogTemp, Warning, TEXT("Option"));
			break;
		}

		if (IsValid(NewPage))
		{
			PageSwitcher->SetActiveWidget(NewPage);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetSwitcher Isn't"));
	}

}

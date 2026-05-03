// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/InGameMenuPanel/InGameMenuWidget.h"
#include "UI/Pannel/BasePannel/InventoryPanelWidget.h"

#include "Component/InventoryComponent_V2.h"
#include "Component/QuestComponent.h"
#include "Component/EquipComponent.h"
#include "AbilitySystemComponent.h"

#include "Shared/Macro/Log.h"

// ---------------- UI ----------------
#include "UI/Inventory/InventoryWidget.h"
#include "UI/Quest/QuestWidget.h"

void UInGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnVisibilityChanged.AddDynamic(this, &ThisClass::ChangedVisibility);
	Init();
}

void UInGameMenuWidget::Init()
{
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		UInventoryComponent_V2* IC = OwningPawn->FindComponentByClass<UInventoryComponent_V2>();
		UEquipComponent* EC = OwningPawn->FindComponentByClass<UEquipComponent>();
		UQuestComponent* QC = OwningPawn->FindComponentByClass<UQuestComponent>();
		UAbilitySystemComponent* ASC = OwningPawn->FindComponentByClass<UAbilitySystemComponent>();

		if (IC && EC && QC && ASC)
		{
			Inventory->Init(IC, EC, ASC);
			Quest->Init(QC);
		}
		else
		{
			MY_LOG(Error, "");
			//check(false);
		}
	}

}



void UInGameMenuWidget::ChangedVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		Quest->Refresh();
	}
}

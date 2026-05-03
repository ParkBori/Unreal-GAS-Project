// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/StatusWidget.h"
#include "UI/Inventory/StatusInfoWidget.h"


void UStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UStatusWidget::Callback_StaminaUpdate(float Value)
{
	Stamina->UpdateCurrentValue(Value);
}

void UStatusWidget::Callback_HealthRegenerationUpdate(float Value)
{
}

void UStatusWidget::Callback_HealthUpdate(float Value)
{
	Health->UpdateCurrentValue(Value);
}

void UStatusWidget::Callback_AdrenalineUpdate(float Value)
{
}

void UStatusWidget::Callback_DamageUpdate(float Value)
{
	Damage->UpdateCurrentValue(Value);
}

void UStatusWidget::Callback_StatusDiffInfoUpdate(const FDiffStatusInfo& NewInfo)
{
		const auto& PreViewEquipStat = NewInfo.PreViewEquipStat;
	const auto& DiffItemOption = NewInfo.DiffItemOption;

	for (const auto& EquipStat : PreViewEquipStat)
	{
		UStatusInfoWidget* StatusInfoWidget = nullptr;
		const FName& Key = EquipStat.Key;

		if (Key == TEXT("Damage"))
		{
			StatusInfoWidget = Damage;
		}
		else if (Key == TEXT("MaxStamina"))
		{
			StatusInfoWidget = Stamina;
		}
		else if (Key == TEXT("MaxHealth"))
		{
			StatusInfoWidget = Health;
		}

		StatusInfoWidget->SetPreViewValue(EquipStat.Value);
	}

	for (const auto& Diff : DiffItemOption)
	{
		UStatusInfoWidget* StatusInfoWidget = nullptr;
		const FName& Key = Diff.Key;

		if (Key == TEXT("Damage"))
		{
			StatusInfoWidget = Damage;
		}
		else if (Key == TEXT("MaxStamina"))
		{
			StatusInfoWidget = Stamina;
		}
		else if (Key == TEXT("MaxHealth"))
		{
			StatusInfoWidget = Health;
		}

		StatusInfoWidget->SetDiffValue(Diff.Value);
	}
}

void UStatusWidget::Callback_UnHoveredEvent(bool bValue)
{
	if (IsValid(Health))
	{
		Health->SetDefault();
	}
	if (IsValid(Stamina))
	{
		Stamina->SetDefault();
	}
	if (IsValid(Damage))
	{
		Damage->SetDefault();
	}
}


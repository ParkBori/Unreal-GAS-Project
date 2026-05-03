// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PlayerHUD.h"
#include "Component/InventoryComponent_V2.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

#include "UI/CustomProgressBar/DoubleProgressBar.h"
#include "UI/CustomProgressBar/OneProgressBar.h"
#include "UI/HUD/GameClearWidget.h"

void UPlayerHUD::Init()
{
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (auto* IC = OwningPawn->FindComponentByClass<UInventoryComponent_V2>())
		{
			// 중복 바인딩 방지를 위해 먼저 Unbind 하거나 체크
			IC->mOnQuickSlotUpdated.RemoveAll(this);
			IC->mOnQuickSlotUpdated.AddDynamic(this, &ThisClass::UpdateQuickSlot);
		}

		if (auto* ASC = OwningPawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			DPB_HP->Init(ASC->GetNumericAttribute(UDefaultAttributeSet::GetHealthAttribute()), ASC->GetNumericAttribute(UDefaultAttributeSet::GetMaxHealthAttribute()));
			
			float Stamina = ASC->GetNumericAttribute(UCharacterAttributeSet::GetStaminaAttribute());
			float MaxStamina = ASC->GetNumericAttribute(UCharacterAttributeSet::GetMaxStaminaAttribute());
			
			OPB_Stamina->Init(ASC->GetNumericAttribute(UCharacterAttributeSet::GetStaminaAttribute()), ASC->GetNumericAttribute(UCharacterAttributeSet::GetMaxStaminaAttribute()));

			ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute()).RemoveAll(this);

			ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute())
				.AddUObject(DPB_HP, &UDoubleProgressBar::ChangedTargetValue);

			ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetMaxHealthAttribute())
				.AddUObject(DPB_HP, &UDoubleProgressBar::OnMaxHealthChanged);

			ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute())
				.AddUObject(OPB_Stamina, &UOneProgressBar::ChangedTargetValue);

			ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxStaminaAttribute())
				.AddUObject(OPB_Stamina, &UOneProgressBar::ChangedMaxValue);

			// 플레이어 체력 및 스태미나가 변경되면 위젯을 자동 활성화 및 자동 
			ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute())
				.AddLambda([this](const FOnAttributeChangeData& ChangeData) {
				SetHUDState(EHUDState::PlayerActive);
					});

			ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute())
				.AddLambda([this](const FOnAttributeChangeData& ChangeData) {
						
				if (FMath::Abs(ChangeData.NewValue - ChangeData.OldValue) > 5.f)
				{
					SetHUDState(EHUDState::PlayerActive);
				}
					});
		}
	}
}

void UPlayerHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetHUDState(mCurrentState);
	// OnVisibilityChanged.AddDynamic(this, &UPlayerHUD::Callback_VisibilityChanged);
}

void UPlayerHUD::NativeDestruct()
{
	if (IsPlayingAnimation())
	{
		StopAllAnimations();
	}

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		OwningPawn->GetWorld()->GetTimerManager().ClearTimer(mVisibilityHandle);

		if (auto* ASC = OwningPawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetMaxStaminaAttribute()).RemoveAll(this);
		}
	}


	Super::NativeDestruct();
}

void UPlayerHUD::UpdateQuickSlot_Implementation(const TArray<FInventoryUISnapshot>& Snapshot)
{
	int a = 3;
}

void UPlayerHUD::ActivateBossAttribute(bool bActivate, AActor* Boss)
{
	if (!IsValid(Boss))
	{
		return;
	}

	if (!BossAttributeAnim)
	{
		return;
	}

	auto* ASC = Boss->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(ASC))
	{
		return;
	}

	if (bActivate && Boss)
	{
		WBP_BossEnemyHP->Init(ASC->GetNumericAttribute(UDefaultAttributeSet::GetHealthAttribute()), ASC->GetNumericAttribute(UDefaultAttributeSet::GetMaxHealthAttribute()));
		WBP_BossEnemyStamina->Init(ASC->GetNumericAttribute(UEnemyAttributeSet::GetGroggyAttribute()), ASC->GetNumericAttribute(UEnemyAttributeSet::GetMaxGroggyAttribute()));

		ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute())
			.AddUObject(WBP_BossEnemyHP, &UDoubleProgressBar::ChangedTargetValue);
		ASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetMaxHealthAttribute())
			.AddUObject(WBP_BossEnemyHP, &UDoubleProgressBar::OnMaxHealthChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetGroggyAttribute())
			.AddUObject(WBP_BossEnemyStamina, &UOneProgressBar::ChangedTargetValue);
		ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxGroggyAttribute())
			.AddUObject(WBP_BossEnemyStamina, &UOneProgressBar::ChangedTargetValue);

		PlayAnimationForward(BossAttributeAnim);
	}
	else
	{
		PlayAnimationReverse(BossAttributeAnim);
	}

}

void UPlayerHUD::ActivateCleaerWidget()
{
	if (!IsValid(ClearWidget))
	{
		return;
	}

	ClearWidget->SetVisibility(ESlateVisibility::Visible);
	ClearWidget->ShowGameCleareWidget();
}



void UPlayerHUD::VisibiltyActivate(bool bValue)
{
	if (bValue)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	SetHUDState(mCurrentState);
}

void UPlayerHUD::SetHUDState(EHUDState State)
{
	switch (State)
	{
	case EHUDState::PlayerActive:
	{
		DPB_HP->SetVisibility(ESlateVisibility::HitTestInvisible);
		OPB_Stamina->SetVisibility(ESlateVisibility::HitTestInvisible);
		WBP_HUDQuickSlots_V3->SetVisibility(ESlateVisibility::HitTestInvisible);

		GetWorld()->GetTimerManager().ClearTimer(mVisibilityHandle);
		GetWorld()->GetTimerManager().SetTimer(mVisibilityHandle, FTimerDelegate::CreateLambda([this]() {
			SetHUDState(EHUDState::PlayerNonActive);
			}), 15.f, false);

	}
		break;
	case EHUDState::PlayerNonActive:
	{
		DPB_HP->SetVisibility(ESlateVisibility::Collapsed);
		OPB_Stamina->SetVisibility(ESlateVisibility::Collapsed);
		WBP_HUDQuickSlots_V3->SetVisibility(ESlateVisibility::Collapsed);
	}
		break;
	}

	mCurrentState = State;
}

//void UPlayerHUD::Callback_VisibilityChanged(ESlateVisibility Visibility)
//{
//	switch (Visibility)
//	{
//	case ESlateVisibility::Visible:
//	case ESlateVisibility::HitTestInvisible:
//	case ESlateVisibility::SelfHitTestInvisible:
//
//		VisibiltyActivate(true);
//
//		break;
//	case ESlateVisibility::Collapsed:
//	case ESlateVisibility::Hidden:
//
//
//		break;
//	}
//}


//애너미 플레이어 그로기 스테미나 리젠 
//공격시 리젠 막기 


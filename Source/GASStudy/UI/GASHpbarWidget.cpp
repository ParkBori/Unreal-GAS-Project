// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASHpbarWidget.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tag/GameplayTag.h"


void UGASHpbarWidget::SetAbilitySystemComponent(AActor* Owner)
{
	Super::SetAbilitySystemComponent(Owner);
	if (mASC)
	{
		// 특정 어트리뷰트가 변경할때 호출되는 함수설정 
		mASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute()).AddUObject(this, &UGASHpbarWidget::OnHealthChanged);
		mASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UGASHpbarWidget::OnMaxHealthChanged);
		// 특정 태그의 변경될때 호출되는 함수설정 
		mASC->RegisterGameplayTagEvent(TAG_CHARACTER_STATE_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASHpbarWidget::OnInvinsibleTagChanged);
		Hpbar->SetFillColorAndOpacity(HealthColor);

		const UDefaultAttributeSet* AttributeSet = mASC->GetSet<UDefaultAttributeSet>();
		if (AttributeSet)
		{
			CurrentHealth = AttributeSet->GetHealth();
			CurrentMaxHealth = AttributeSet->GetMaxHealth();
			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpbar();
			}
		}
	}
}

void UGASHpbarWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealth = Data.NewValue;
	UpdateHpbar();
}

void UGASHpbarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentMaxHealth = Data.NewValue;
	UpdateHpbar();
}

void UGASHpbarWidget::UpdateHpbar()
{
	if (Hpbar)
	{
		Hpbar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TextHpStat)
	{
		TextHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, CurrentMaxHealth)));
	}
}

void UGASHpbarWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Hpbar->SetFillColorAndOpacity(InvinsibleColor);
		Hpbar->SetPercent(1.0f);
	}
	else
	{
		Hpbar->SetFillColorAndOpacity(HealthColor);
		UpdateHpbar();
	}
}

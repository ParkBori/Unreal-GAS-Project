// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/EnemyAttributeWidget.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"



void UEnemyAttributeWidget::SetAbilitySystemComponent(AActor* Owner)
{
	Super::SetAbilitySystemComponent(Owner);
	if (mASC)
	{
		mASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetHealthAttribute()).AddUObject(this, &UEnemyAttributeWidget::OnHealthChanged);
		mASC->GetGameplayAttributeValueChangeDelegate(UDefaultAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UEnemyAttributeWidget::OnMaxHealthChanged);

		const UDefaultAttributeSet* AttributeSetDefault = mASC->GetSet<UDefaultAttributeSet>();
		if (AttributeSetDefault)
		{
			mCurrentHp = AttributeSetDefault->GetHealth();
			mMaxHp = AttributeSetDefault->GetMaxHealth();
			if (mMaxHp > 0.0f)
			{
				UpdateHpbar();
			}
		}

		mASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetGroggyAttribute()).AddUObject(this, &UEnemyAttributeWidget::OnGroggyhChanged);
		mASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxGroggyAttribute()).AddUObject(this, &UEnemyAttributeWidget::OnMaxGroggyChanged);

		const UEnemyAttributeSet* EnemyAttributeSet = mASC->GetSet<UEnemyAttributeSet>();
		if (EnemyAttributeSet)
		{
			mCurrentGroggy = EnemyAttributeSet->GetGroggy();
			mMaxGroggy = EnemyAttributeSet->GetMaxGroggy();
			if (mMaxHp > 0.0f)
			{
				UpdateGroggybar();
			}
		}
	}
}

void UEnemyAttributeWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	mCurrentHp = Data.NewValue;
	UpdateHpbar();
}

void UEnemyAttributeWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	mMaxHp = Data.NewValue;
	UpdateHpbar();
}

void UEnemyAttributeWidget::UpdateHpbar()
{
	if (!PB_Hp)
	{
		return;
	}
	
	PB_Hp->SetPercent(mCurrentHp / mMaxHp);
}

void UEnemyAttributeWidget::OnGroggyhChanged(const FOnAttributeChangeData& Data)
{
	mCurrentGroggy = Data.NewValue;
	UpdateGroggybar();
}

void UEnemyAttributeWidget::OnMaxGroggyChanged(const FOnAttributeChangeData& Data)
{
	mMaxGroggy = Data.NewValue;
	UpdateGroggybar();
}

void UEnemyAttributeWidget::UpdateGroggybar()
{
	if (!PB_Groggy)
	{
		return;
	}

	PB_Groggy->SetPercent(mCurrentGroggy/ mMaxGroggy);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Shared/Tag/SharedTag.h"

#include "AbilitySystemBlueprintLibrary.h"


void UDefaultAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UDefaultAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		if (OldValue > 0.f && !FMath::IsNearlyEqual(OldValue, NewValue))
		{
			float CalculatedCurrentValue = AdjustRatio(GetHealth(), OldValue, NewValue);
			SetHealth(CalculatedCurrentValue);
		}
	}

	if (Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= 0)
		{
			GetOwningAbilitySystemComponent()->AddLooseGameplayTag(State::State_Dead);
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), Event::Event_Ability_Death, FGameplayEventData());
		}
	}
}

void UDefaultAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetDamage(FMath::Clamp(GetDamage(), 0.0f, Data.EvaluatedData.Magnitude));
	}

	if (Data.EvaluatedData.Attribute == GetDefenseAttribute())
	{
		SetDefense(FMath::Clamp(GetDefense(), 0.0f, Data.EvaluatedData.Magnitude));
	}

	/*if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() < 0)
		{
			SetHealth(0.f);
			GetOwningAbilitySystemComponent()->AddLooseGameplayTag(State::State_Dead);
		}
	}*/
}


void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxStaminaAttribute())
	{
		if (OldValue > 0.f && !FMath::IsNearlyEqual(OldValue, NewValue))
		{
			float CalculatedCurrentValue = AdjustRatio(GetStamina(), OldValue, NewValue);
			SetStamina(CalculatedCurrentValue);
		}
	}
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetGroggyAttribute())
	{
		SetGroggy(FMath::Clamp(GetGroggy(), 0.0f, GetMaxGroggy()));

		if (GetGroggy() <= 0.f)
		{
			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			if (!ASC)
			{
				return;
			}

			// 이미 그로기 상태면 중복 방지
			if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Groggy"))))
			{
				return;
			}

			// GA_Groggy 실행
			//ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(Ability::Ability_Combat_Groggy));
		}
	}
}


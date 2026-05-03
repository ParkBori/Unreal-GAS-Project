// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

UAbilitySystemComponent* UGASUserWidget::GetAbilitySystemComponent() const
{
	return mASC;
}

void UGASUserWidget::SetAbilitySystemComponent(AActor* Owner)
{
	if (IsValid(Owner))
	{
		mASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	}
}

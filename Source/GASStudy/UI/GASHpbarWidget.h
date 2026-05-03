// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "GASHpbarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UGASHpbarWidget : public UGASUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* Owner) override;
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void UpdateHpbar();

	void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> Hpbar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};

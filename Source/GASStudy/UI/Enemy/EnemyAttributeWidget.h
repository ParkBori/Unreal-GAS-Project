// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "EnemyAttributeWidget.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class GASSTUDY_API UEnemyAttributeWidget : public UGASUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* Owner) override;
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void UpdateHpbar();
	
	void OnGroggyhChanged(const FOnAttributeChangeData& Data);
	void OnMaxGroggyChanged(const FOnAttributeChangeData& Data);
	void UpdateGroggybar();


protected:
	float mCurrentHp;
	float mMaxHp;

	float mCurrentGroggy;
	float mMaxGroggy;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Hp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> S_Hp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Groggy;
};

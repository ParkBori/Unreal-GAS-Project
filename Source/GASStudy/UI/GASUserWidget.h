// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "AbilitySystemInterface.h"
#include "GASUserWidget.generated.h"

/**
 * GASWidgetComponent 에 지정해서 사용하는 UI 전용
 * GASWidgetComponent 에 담기지 않으면 mASC 는 초기화 되지않음!!
 */
UCLASS()
class GASSTUDY_API UGASUserWidget 
	: public UUserWidget
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void SetAbilitySystemComponent(AActor* Owner);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAbilitySystemComponent> mASC;
};

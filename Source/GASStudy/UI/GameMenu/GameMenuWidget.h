// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameMenuPublicInfo.h"
#include "GameMenuWidget.generated.h"

/**
 * 
 */


UCLASS()
class GASSTUDY_API UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UGameMenuWidget(const FObjectInitializer& ObjectInitializer);

	
protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void UpdatePage(EGameMenuType NewType);

public:

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UGameMenuButtonWidget>> WidgetSwitchButtons;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetSwitcher> PageSwitcher;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> EquipPage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> InventoryPage;
	
	UPROPERTY(EditAnywhere)
	EGameMenuType CurrentType = EGameMenuType::EQUIP;
};

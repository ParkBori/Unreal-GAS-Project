// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Item/ItemInfo.h"
#include "StatusWidget.generated.h"

/**
 * 
 */

class UStatusInfoWidget;

UCLASS()
class GASSTUDY_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintCallable)
	void Callback_StaminaUpdate(float Value);

	UFUNCTION(BlueprintCallable)
	void Callback_HealthRegenerationUpdate(float Value);

	UFUNCTION(BlueprintCallable)
	void Callback_HealthUpdate(float Value);

	UFUNCTION(BlueprintCallable)
	void Callback_AdrenalineUpdate(float Value);

	UFUNCTION(BlueprintCallable)
	void Callback_DamageUpdate(float Value);

	UFUNCTION(BlueprintCallable)
	void Callback_StatusDiffInfoUpdate(const FDiffStatusInfo& NewInfo);

	UFUNCTION(BlueprintCallable)
	void Callback_UnHoveredEvent(bool bValue);
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatusInfoWidget> Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusInfoWidget> Stamina;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatusInfoWidget> Damage;


	// UPROPERTY()
	// TSubclassOf<class UStatusInfoWidget> mStatusInfoWidgetClass;
};

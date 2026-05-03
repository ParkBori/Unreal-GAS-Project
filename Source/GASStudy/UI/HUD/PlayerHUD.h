// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "UI/BaseUserWidget.h"
#include "PlayerHUD.generated.h"

class UDoubleProgressBar;
class UOneProgressBar;

enum class EHUDState : uint8
{
	PlayerActive,
	PlayerNonActive,
};


/**
 * 
 */
UCLASS()
class GASSTUDY_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateQuickSlot(const TArray<FInventoryUISnapshot>& Snapshot);
	void UpdateQuickSlot_Implementation(const TArray<FInventoryUISnapshot>& Snapshot);

	UFUNCTION(BlueprintCallable)
	void ActivateBossAttribute(bool bActivate, AActor* Boss);

	UFUNCTION(BlueprintCallable)
	void ActivateCleaerWidget();

	void VisibiltyActivate(bool bValue);


private:
	//UFUNCTION()
	//void Callback_VisibilityChanged(ESlateVisibility Visibility);
	void SetHUDState(EHUDState State);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHUDQuestTrackerWidget> QuestTracker;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDoubleProgressBar> DPB_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOneProgressBar> OPB_Stamina;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadWrite)
	TObjectPtr<UUserWidget> WBP_HUDQuickSlots_V3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDoubleProgressBar> WBP_BossEnemyHP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOneProgressBar> WBP_BossEnemyStamina;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> BossAttributeAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGameClearWidget> ClearWidget;

	EHUDState mCurrentState = EHUDState::PlayerNonActive;

	bool bIsActivePlayerInfo = false;

	FTimerHandle mVisibilityHandle;
};



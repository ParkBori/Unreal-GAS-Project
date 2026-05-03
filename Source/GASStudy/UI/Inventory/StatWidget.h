// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "Shared/Inventory/Snapshot.h"
#include "StatWidget.generated.h"

/**
 * 
 */

class UEquipComponent;
class UAbilitySystemComponent;

UCLASS()
class GASSTUDY_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(UEquipComponent* InventoryComponent, UAbilitySystemComponent* AbilitySystemComponent);
	
protected:
	virtual void NativePreConstruct() override;


protected:
	void UpdateStats(const FStatSnapshotMap& Snapshot);

	void UpdateStats(UAbilitySystemComponent* ASC, const struct FGameplayEffectSpec& Spec, struct FActiveGameplayEffectHandle Handle);
	void UpdateEffectRemoved(const struct FActiveGameplayEffect& Effect);


private:
	void AddVerticalBox(
		UWidget* Widget,
		const FSlateChildSize& SlateChildSize,
		const EHorizontalAlignment& HorizontalAlignment,
		const EVerticalAlignment& VerticalAlignment,
		const FMargin& Margin);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_Stat;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FStatUISnapshot> CurrentStatSnapshot;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UStatusInfoWidget> mInfoWidget;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, TObjectPtr<UStatusInfoWidget>> mStatInfoWidgets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> mImgTexture;

	//UPROPERTY(BlueprintReadOnly)
	//TMap<FName, FStatUISnapshot> mStat  

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UEquipComponent> mEquipComponent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> mAbilitySystemComponent;
};

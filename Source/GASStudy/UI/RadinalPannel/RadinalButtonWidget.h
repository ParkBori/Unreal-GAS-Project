// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "RadinalButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateAbility, const FGameplayTag&, InTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoveredButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnHoveredButton);

/**
 * 
 */
UCLASS()
class GASSTUDY_API URadinalButtonWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized() override;


public:
	void SetBtnIdx(int Value)
	{
		mBtnIdx = Value;
	}

private:
	UFUNCTION()
	void OnClickedCallback_Btn_Trigger();
	
	UFUNCTION()
	void OnHoveredCallback_Btn_Trigger();

	UFUNCTION()
	void OnUnHoveredCallback_Btn_Trigger();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString mText;

	UPROPERTY(EditAnywhere)
	int32 mBtnIdx;

	UPROPERTY(EditAnywhere)
	FGameplayTag mAbilityTag;

public:
	UPROPERTY(BlueprintAssignable)
	FOnClickButton mOnClickButton;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredButton mOnHoveredButton;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredButton mOnUnHoveredButton;

	UPROPERTY(BlueprintAssignable)
	FOnActivateAbility mOnActivateAbility;
};

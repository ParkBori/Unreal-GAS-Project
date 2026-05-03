// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "CreditWidget.generated.h"


UENUM(BlueprintType)
enum class ECreditType : uint8
{
	ASSET,
	PROGREMER,
	ETC,
};

UCLASS()
class GASSTUDY_API UCreditInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<FString> CreditMessage;

public:
	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
	TMap<ECreditType, FText> CreditMessage;
};

/**
 * 
 */
UCLASS()
class GASSTUDY_API UCreditWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCreditWidget(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnClickedCallback_Btn_Close();

private:
	void CreditEnd();

	UFUNCTION()
	void TransitionMainLevel();


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SB_ScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Close;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCreditInfo> mDA_CreditInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> mAlphaCurv;

	UPROPERTY(EditDefaultsOnly)
	float mScrollSpeed = 0.1f;

	float mScrollInterpValue = 0.f;

	float mAccDelta = 0.f;

	FTimerHandle mTransitionMainLevelHandle;

	UPROPERTY(EditAnywhere)
	float mWorldTransitionDelay = 5.f;
};

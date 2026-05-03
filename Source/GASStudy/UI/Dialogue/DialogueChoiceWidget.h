// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIInfo.h"
#include "Shared/Dialogue/DialogueData.h"
#include "DialogueChoiceWidget.generated.h"



/**
 * 
 */
UCLASS()
class GASSTUDY_API UDialogueChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

public:
	void Init(const FUIChoiceData& Data);
	
private:
	UFUNCTION()
	void Btn_Choice_Clicked();

private:
	UFUNCTION(BlueprintCallable)
	void ChangeTextStyle(bool bHover);


public:

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_Script;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_Number;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Choice;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonTextStyle> mHoverTextStyle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonTextStyle> mUnHoverTextStyle;


private:
	UPROPERTY()
	FUIChoiceData mChoiceData;
};

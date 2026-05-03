// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicButtonsWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UDynamicButtonsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:
	virtual void NativePreConstruct() override;
	
	
};

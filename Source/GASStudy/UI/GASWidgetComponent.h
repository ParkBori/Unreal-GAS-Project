// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GASWidgetComponent.generated.h"

/**
 * UGASUserWidget 하위 클래스만 사용할 수 있음!!
 * InitWidget 에서 해당컴포넌트의 오너의 ASC 를 얻어어고,
 * UGASUserWidget 에 ASC 를 할당
 */
UCLASS()
class GASSTUDY_API UGASWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget() override;
};

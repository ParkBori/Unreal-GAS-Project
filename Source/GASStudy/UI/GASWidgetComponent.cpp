// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASWidgetComponent.h"
#include "UI/GASUserWidget.h"

void UGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 위젯컴포넌트가 소유한 위젯을 GASWidget 으로 캐스팅 
	UGASUserWidget* GASUserWidget = Cast<UGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}

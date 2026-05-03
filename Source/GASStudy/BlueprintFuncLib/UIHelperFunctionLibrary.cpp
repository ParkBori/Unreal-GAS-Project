// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFuncLib/UIHelperFunctionLibrary.h"

#include "Blueprint/UserWidget.h"

#include "UIHelperFunctionLibrary.h"

#include "UI/Popup/PopupWidget.h"

UMaterialInstanceDynamic* UUIHelperFunctionLibrary::CreateDynamicMaterilFrom(const FSlateBrush& InBrush, UObject* InOuter)
{
	UMaterialInstance* MatInst = Cast<UMaterialInstance>(InBrush.GetResourceObject());
	if (IsValid(MatInst))
	{
		UMaterialInstanceDynamic* DynamicMI= UMaterialInstanceDynamic::Create(MatInst, InOuter);
		return DynamicMI;
	}

	return nullptr;
}

UWidgetAnimation* UUIHelperFunctionLibrary::FindAnimation(UUserWidget* Widget, const FString& AnimName)
{
	UWidgetBlueprintGeneratedClass* WidgetClass = Widget->GetWidgetTreeOwningClass();

	FString NewName = AnimName + TEXT("_INST");
	for (auto& Anim : WidgetClass->Animations)
	{
		FString	Name = Anim->GetName();

		if (NewName == Name)
		{
			return Anim;
		}
	}

	return nullptr;
}

UWidgetAnimation* UUIHelperFunctionLibrary::PlayWidgetAnimation(UUserWidget* Widget, const FString& AnimName, float PlayStartTime, float PlaySpeed, bool Forward, bool RestoreState, int32 LoopCount)
{
	UWidgetAnimation* Anim = FindAnimation(Widget, AnimName);
	if (IsValid(Anim))
	{
		EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward;

		if (!Forward)
			PlayMode = EUMGSequencePlayMode::Reverse;

		Widget->PlayAnimation(Anim, PlayStartTime, LoopCount, PlayMode,
			PlaySpeed, RestoreState);
	}

	return Anim;
}

UPopupWidget* UUIHelperFunctionLibrary::CreatePopup(FText Title, FText Script)
{


	return nullptr;
}

UPopupWidget* UUIHelperFunctionLibrary::CreatePopupTimer(FText Title, FText Script, float Time)
{
	return nullptr;
}

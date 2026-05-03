// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/GameMenuButtonWidget.h"

UGameMenuButtonWidget::UGameMenuButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameMenuButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("ButtonHitBox"))));
	Button->OnHovered.AddDynamic(this, &UGameMenuButtonWidget::HoverButton);
	Button->OnUnhovered.AddDynamic(this, &UGameMenuButtonWidget::UnHoverButton);
}

void UGameMenuButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	for (auto& Anim : WidgetClass->Animations)
	{
		FString	Name = Anim->GetName();
		Name.ReplaceInline(TEXT("_INST"), TEXT(""), ESearchCase::IgnoreCase);

		ButtonAnimation.Add(Name, Anim);
	}
}

void UGameMenuButtonWidget::PlayWidgetAnimation(const FString& Name, float PlayStartTime, float PlaySpeed, bool Forward, bool RestoreState, int32 LoopCount)
{
	TObjectPtr<UWidgetAnimation> Anim = ButtonAnimation.FindRef(Name);
	if (IsValid(Anim))
	{
		EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward;
		if (!Forward)
			PlayMode = EUMGSequencePlayMode::Reverse;

		PlayAnimation(Anim, PlayStartTime, LoopCount, PlayMode,
			PlaySpeed, RestoreState);
	}
}

void UGameMenuButtonWidget::HoverButton()
{
	UE_LOG(LogTemp, Display, TEXT("Hover"));
	PlayWidgetAnimation(TEXT("Select"));

	OnButtonSelect.ExecuteIfBound(ButtonType);
}

void UGameMenuButtonWidget::UnHoverButton()
{
	UE_LOG(LogTemp, Display, TEXT("UnHover"));
	PlayWidgetAnimation(TEXT("Select"), 0.f, 1.f, false);
}

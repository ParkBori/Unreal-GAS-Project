// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/StatWidget.h"

#include "Component/EquipComponent.h"
#include "UI/Inventory/StatusInfoWidget.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

void UStatWidget::Init(UEquipComponent* EquipComponent, UAbilitySystemComponent* AbilitySystemComponent)
{
	mEquipComponent = EquipComponent;
	mEquipComponent->mOnStatUpdated.AddUObject(this, &UStatWidget::UpdateStats);
	
	mAbilitySystemComponent = AbilitySystemComponent;
}

void UStatWidget::NativePreConstruct()
{
	// 이시점에는 Init 이 실행된 이후의 시점이다.
	Super::NativePreConstruct();
	
	// 어트리뷰트만큼, 위젯 생성해주기
	auto PC = GetOwningPlayer<APlayerController>();
	if (nullptr == PC)
	{
		MY_LOG(Error, "PlayerController is nullptr");
		return;
	}

	int Cnt = 0;
	TArray<FGameplayAttribute> Attributes;
	mAbilitySystemComponent->GetAllAttributes(Attributes);
	for (auto& Attribute : Attributes)
	{
		UStatusInfoWidget* InfoWidget = CreateWidget<UStatusInfoWidget>(PC, mInfoWidget);
		FStatUISnapshot Snapshot;
		Snapshot.CurrentValue = mAbilitySystemComponent->GetNumericAttribute(Attribute);

		InfoWidget->Init(Attribute.GetName(), Snapshot);
		InfoWidget->SetCurrentValue();

		AddVerticalBox(
			InfoWidget, 
			ESlateSizeRule::Automatic, 
			EHorizontalAlignment::HAlign_Fill, 
			EVerticalAlignment::VAlign_Fill, 
			FMargin(15.f, 0.f, 15.f, 5.f));

	
		mStatInfoWidgets.Add(*Attribute.GetName(), InfoWidget);

		Cnt++;
		if (Cnt % 2 == 0)
		{
			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetHeightOverride(2.f);
			UImage* Img = NewObject<UImage>(this);
			Img->SetBrushFromTexture(mImgTexture);

			USizeBoxSlot* SBS = Cast<USizeBoxSlot>(SizeBox->AddChild(Img));
			if (SBS)
			{
				SBS->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				SBS->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}

			AddVerticalBox(
				SizeBox,
				ESlateSizeRule::Automatic,
				EHorizontalAlignment::HAlign_Fill,
				EVerticalAlignment::VAlign_Fill,
				FMargin(15.f));
		}


		mAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
			.AddUObject(InfoWidget, &UStatusInfoWidget::CallbackAttributeChanged);
	}
}

void UStatWidget::UpdateStats(const FStatSnapshotMap& Snapshot)
{
	// 여기서 호버 됐는지에 대한 정보 변화가 필요함
	for (const auto& S : Snapshot)
	{
		const FName& Key = *S.Key.GetName();
		if (mStatInfoWidgets.Contains(Key))
		{
			MY_LOG(Warning, "%s", *S.Value.ToString());
			mStatInfoWidgets[Key]->Update(S.Value);
		}
	}
}

void UStatWidget::UpdateStats(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	if (!ASC) return;

	TArray<FGameplayAttribute> Attributes;
	ASC->GetAllAttributes(Attributes);

	for (const auto& Attribute : Attributes)
	{
		if (mStatInfoWidgets.Contains(*Attribute.GetName()))
		{
			float FinalValue = ASC->GetNumericAttribute(Attribute);

			mStatInfoWidgets[*Attribute.GetName()]->UpdateValue(FinalValue);
		}
	}
}

void UStatWidget::UpdateEffectRemoved(const FActiveGameplayEffect& Effect)
{
	TArray<FGameplayAttribute> Attributes;
	mAbilitySystemComponent->GetAllAttributes(Attributes);

	for (const auto& Attribute : Attributes)
	{
		mStatInfoWidgets[*Attribute.GetName()]->UpdateValue(mAbilitySystemComponent->GetNumericAttribute(Attribute));
	}
}

void UStatWidget::AddVerticalBox(UWidget* Widget, const FSlateChildSize& SlateChildSize, const EHorizontalAlignment& HorizontalAlignment, const EVerticalAlignment& VerticalAlignment, const FMargin& Margin)
{
	UVerticalBoxSlot* VBS = VB_Stat->AddChildToVerticalBox(Widget);
	if (VBS)
	{
		VBS->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		VBS->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		VBS->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		VBS->SetPadding(FMargin(15.f, 0.f, 15.f, 5.f));
	}
}



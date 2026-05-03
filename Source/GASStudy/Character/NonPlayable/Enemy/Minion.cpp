// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NonPlayable/Enemy/Minion.h"
#include "UI/GASWidgetComponent.h"

AMinion::AMinion()
{
	mGASWidgetComp = CreateDefaultSubobject<UGASWidgetComponent>(TEXT("GASWidgetComponent"));
	if (IsValid(mGASWidgetComp))
	{

		if (GetMesh()->DoesSocketExist(TEXT("HealthBar")))
		{
			mGASWidgetComp->SetupAttachment(GetMesh(), TEXT("HealthBar"));
		}
		mGASWidgetComp->SetWidgetSpace(EWidgetSpace::World);
		mGASWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mGASWidgetComp->SetDrawSize(FVector2D(152, 20));
		mGASWidgetComp->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
		mGASWidgetComp->SetAbsolute(false, true, false);


	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	WidgetMtrl(TEXT("/Script/Engine.Material'/Game/GASStudy/Materials/M_HeadInfo.M_HeadInfo'"));
	if (WidgetMtrl.Succeeded())
	{
		mWidgetMaterial = WidgetMtrl.Object;
	}
}

void AMinion::BeginPlay()
{
	Super::BeginPlay();

	mGASWidgetComp->SetMaterial(0, mWidgetMaterial);
}

void AMinion::EventDeath(const FGameplayTag Tag, int32 NewCount)
{
	Super::EventDeath(Tag, NewCount);

	if (NewCount >= 1)
	{
		InteractQuest();
	}

	mGASWidgetComp->SetVisibility(false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NonPlayable/NPC/NPC.h"
#include "Component/DialogueComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"



ANPC::ANPC()
{
	mDialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
	mTriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	mTriggerComponent->SetupAttachment(RootComponent);
	
	mSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	mSpringArmComponent->SetupAttachment(RootComponent);

	mCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	mCameraComponent->SetupAttachment(mSpringArmComponent);
	
	//mCameraComponent->bConstrainAspectRatio = true;
	//mCameraComponent->AspectRatio = 2.39f;
}


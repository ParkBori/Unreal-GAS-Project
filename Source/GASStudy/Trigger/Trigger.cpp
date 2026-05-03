// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/Trigger.h"
#include "Components/BoxComponent.h"

// Sets default values
ATrigger::ATrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}

void ATrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (mBoxComp)
	{
		mBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATrigger::Callback_TriggerOverlap);
	}
}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();
}

void ATrigger::Callback_TriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActivateTriggerEvent(OtherActor);

	mOnTriggerOverlapped.Broadcast();
}

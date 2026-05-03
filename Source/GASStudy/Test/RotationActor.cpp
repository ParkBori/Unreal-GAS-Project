// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/RotationActor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RotationActor.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
//#include "GameplayAbility/GA_Rotate.h"
#include "Tag/GameplayTag.h"
#include "Abilities/GameplayAbility.h"

// Sets default values
ARotationActor::ARotationActor()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));


	ActionPeriod = 3.0f;
}

// Called when the game starts or when spawned
void ARotationActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ARotationActor::TimerAction, ActionPeriod, true, 0.0f);
}

void ARotationActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();

	ASC->InitAbilityActorInfo(this, this);
	

	for (const auto& StartAbility : StartAbilities)
	{
		// 부화를 줄이기 위해서 인스턴스를 생성하지않고 정보만 받음 
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
	
}

UAbilitySystemComponent* ARotationActor::GetAbilitySystemComponent() const
{
	return ASC;
}

void ARotationActor::TimerAction()
{
	//FGameplayAbilitySpec* GA_RotateSpec = ASC->FindAbilitySpecFromClass(UGA_Rotate::StaticClass());


	FGameplayTagContainer TargetTag(TAG_ACTOR_ROATE);

	if (!ASC->HasMatchingGameplayTag(TAG_ACTOR_ROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}


	/*if (!GA_RotateSpec)
	{
		UE_LOG(LogTemp, Error, TEXT("Spec not exist"));
		return;
	}
	
	if (!GA_RotateSpec->IsActive())
	{
		ASC->TryActivateAbility(GA_RotateSpec->Handle);
	}
	else
	{
		ASC->CancelAbilityHandle(GA_RotateSpec->Handle);

	}*/

	//if (RotatingMovement->IsActive())
	//{
	//	RotatingMovement->Deactivate();
	//}
	//else
	//{
	//	RotatingMovement->Activate(true);
	//}
}

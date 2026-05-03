// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractableActor.h"
#include "Components/BoxComponent.h"



// Sets default values	
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	mDefualtSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(mDefualtSceneRoot);

	mCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	mCollision->SetupAttachment(mDefualtSceneRoot);

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	mStaticMesh->SetupAttachment(mDefualtSceneRoot);
}

void AInteractableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//mStaticMesh->SetCustomDepthStencilValue((int32)mInteractCustomStencil);

	SetStencilValue();
}

void AInteractableActor::ToggleOutline_Implementation(bool bActivate)
{
	if (!bCanInteract)
	{
		return;
	}

	// UE_LOG(LogTemp, Error, TEXT("Outline : %s"), bActivate ? TEXT("true") : TEXT("false"));
	for (const auto& Mesh : mApplyStencilMesh)
	{
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(bActivate);
		}
	}
}

void AInteractableActor::SetStencilValue()
{
	for (const auto& Mesh : mApplyStencilMesh)
	{
		if (Mesh)
		{
			Mesh->SetCustomDepthStencilValue((int32)mInteractCustomStencil);
		}
	}
}



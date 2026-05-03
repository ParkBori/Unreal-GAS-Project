// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/WorldItemActor.h"
#include "DeveloperSettings/GameDataSettings.h"
#include "Shared/Item/ItemInfo.h"
#include "Subsystem/ItemSubsystem.h"
#include "NiagaraFunctionLibrary.h"



void AWorldItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mNiagara))
	{
		mNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			mNiagara,
			GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true // bAutoDestroy
		);
	}
}


void AWorldItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bUpdateMeshByID)
	{
		const FItemInfoBase* ItemData = UGameDataSettings::Get()->GetDataTableRow<FItemInfoBase>(EGlobalGameDataTableType::ItemData, mItemID);
		if (ItemData && ItemData->ItemData)
		{
			if (ItemData->ItemData->StaticMesh)
			{
				mStaticMesh->SetStaticMesh(ItemData->ItemData->StaticMesh);
			}
		}
	}
}

void AWorldItemActor::Interact_Implementation(AActor* Actor)
{
	auto* SS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (SS)
	{
		if (SS->RequestAddItem(Actor, mItemID, 1))
		{
			if (mNiagaraComp)
			{
				mNiagaraComp->DestroyComponent();
			}

			Destroy();
		}
	}
}




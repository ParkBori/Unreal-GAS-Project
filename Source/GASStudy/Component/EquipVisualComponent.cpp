// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipVisualComponent.h"
#include "Shared/Equip/EquipData.h" 

#include "Shared/Tag/SharedTag.h"


// Sets default values for this component's properties
UEquipVisualComponent::UEquipVisualComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;

	// ...
}

void UEquipVisualComponent::InitializeComponent()
{
    Super::InitializeComponent();

    TArray<USkeletalMeshComponent*> SkeletalMeshComps;
    GetOwner()->GetComponents<USkeletalMeshComponent>(SkeletalMeshComps);

    for (USkeletalMeshComponent* Comp : SkeletalMeshComps)
    {
        mCachedSkeletalMeshComponents.Add(Comp->GetFName(), Comp);
    }


    TArray<UStaticMeshComponent*> StaticMeshComps;
    GetOwner()->GetComponents<UStaticMeshComponent>(StaticMeshComps);

    for (UStaticMeshComponent* Comp : StaticMeshComps)
    {
        mCachedStaticMeshComponents.Add(Comp->GetFName(), Comp);
    }
}

//여기서 정제된 정보를 받고있는데.. 정보를 통으로 받아서 내부에서 처리하면 되지않을까? 

void UEquipVisualComponent::ApplyEquipment(const FGameplayTag& SlotTag, const USkeletalMesh* SkeltalMesh, const UStaticMesh* StaticMesh)
{
    UE_LOG(LogTemp, Error, TEXT("SlotTag = %s"), *SlotTag.ToString());

    if (mCachedSkeletalMeshComponents.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Component isnt"));
        return;
    }

    if (!mVisualMappingData)
    {
        return;
    }

    const FEquipmentVisualData* VisualData =
        mVisualMappingData->SlotToVisualMap.Find(SlotTag);

    if (!VisualData) 
    {
        UE_LOG(LogTemp, Error, TEXT("SKip"));
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("SlotTag = %s"), *VisualData->TargetComponentName.ToString());

    USkeletalMeshComponent** FoundSkeletalMesh =
        mCachedSkeletalMeshComponents.Find(VisualData->TargetComponentName);
    if (FoundSkeletalMesh && *FoundSkeletalMesh)
    {
        (*FoundSkeletalMesh)->SetSkeletalMesh(const_cast<USkeletalMesh*>(SkeltalMesh));
        return;
    }

    UStaticMeshComponent** FoundStaticMesh =
        mCachedStaticMeshComponents.Find(VisualData->TargetComponentName);
    if (FoundStaticMesh && *FoundStaticMesh)
    {
        (*FoundStaticMesh)->SetStaticMesh(const_cast<UStaticMesh*>(StaticMesh));
        return;
    }
}

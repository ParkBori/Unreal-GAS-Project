// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "EquipVisualComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UEquipVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipVisualComponent();

	virtual void InitializeComponent() override;

public:	
	void ApplyEquipment(const FGameplayTag& SlotTag, const USkeletalMesh* SkeltalMesh, const UStaticMesh* StaticMesh);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UEquipmentVisualDataAsset> mVisualMappingData;

	UPROPERTY()
	TMap<FName, USkeletalMeshComponent*> mCachedSkeletalMeshComponents;

	UPROPERTY()
	TMap<FName, UStaticMeshComponent*> mCachedStaticMeshComponents;

};

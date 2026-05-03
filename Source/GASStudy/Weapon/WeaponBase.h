// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Weapon/WeaponData.h"
#include "WeaponBase.generated.h"


class UWeaponTraceComponent;

UCLASS()
class GASSTUDY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

public:
	UFUNCTION(BlueprintCallable, Category = "Equip")
	const FName& GetAttachSocket() const
	{
		return mAttachSocket;
	}

private:
	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (DisplayName = "WeaponStaticMesh"))
	TObjectPtr<UStaticMeshComponent> mWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Trace", meta = (DisplayName = "WeaponTraceComponent"))
	FName mAttachSocket;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mWeaponMesh"));
	mWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(mWeaponMesh);

}


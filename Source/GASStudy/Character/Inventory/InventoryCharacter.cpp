// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Inventory/InventoryCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/EquipVisualComponent.h"
#include "Tag/GameplayTag.h"
#include "Subsystem/InventoryPreviewSubsystem.h"

// Sets default values
AInventoryCharacter::AInventoryCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	mSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	mSpringArmComp->SetupAttachment(RootComponent);
	mCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamereaComponent"));
	mCameraComp->SetupAttachment(mSpringArmComp);
	mWeaponStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("WeaponStaticMeshComponent");
	mWeaponStaticMeshComp->SetupAttachment(GetMesh(), TEXT("weapon_start"));
	mEquipVisualComp = CreateDefaultSubobject<UEquipVisualComponent>(TEXT("EquipVisualComponent"));
}

void AInventoryCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto* IPS = GetWorld()->GetSubsystem<UInventoryPreviewSubsystem>();
	if (IPS)
	{
		IPS->SetPreviewCharacter(this);
	}
}

// Called when the game starts or when spawned
void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AInventoryCharacter::ApplyEquipmentBySlotTag(const FGameplayTag& SlotTag, const USkeletalMesh* ItemMesh)
{
	using EquipFunc = void (AInventoryCharacter::*)(const USkeletalMesh*);

	static TMap<FGameplayTag, EquipFunc> EquipActions;

	if (EquipActions.Num() == 0)
	{
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_CURIASS, &AInventoryCharacter::EquipCuirass);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_HEAD, &AInventoryCharacter::EquipHelmet);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_MASK, &AInventoryCharacter::EquipMask);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_PANTS, &AInventoryCharacter::EquipPants);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_BELT, &AInventoryCharacter::EquipBelt);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_BOOTS, &AInventoryCharacter::EquipBoots);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_ARMOR_GLOVES, &AInventoryCharacter::EquipGloves);
		EquipActions.Add(TAG_ITEM_EQUIPMENT_WEAPON, &AInventoryCharacter::EquipWeapon);
	}

	if (const auto* Func = EquipActions.Find(SlotTag))
	{
		(this->* * Func)(ItemMesh);
	}
}
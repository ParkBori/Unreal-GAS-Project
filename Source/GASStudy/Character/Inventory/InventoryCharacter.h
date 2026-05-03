// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shared/Item/ItemInfo.h"
#include "InventoryCharacter.generated.h"

UCLASS()
class GASSTUDY_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInventoryCharacter();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void ApplyEquipmentBySlotTag(const FGameplayTag& SlotTag, const USkeletalMesh* Mesh);


	UFUNCTION(BlueprintImplementableEvent)
	void EquipBelt(const USkeletalMesh* ItemMesh);
	UFUNCTION(BlueprintImplementableEvent)
	void EquipHelmet(const USkeletalMesh* ItemMesh);
	UFUNCTION(BlueprintImplementableEvent)
	void EquipMask(const USkeletalMesh* ItemMesh);
	UFUNCTION(BlueprintImplementableEvent)
	void EquipCuirass(const USkeletalMesh* ItemMesh);
	UFUNCTION(BlueprintImplementableEvent)
	void EquipGloves(const USkeletalMesh* ItemMesh);
	UFUNCTION(BlueprintImplementableEvent)
	void EquipPants(const USkeletalMesh* ItemMesh);
	UFUNCTION(BlueprintImplementableEvent)
	void EquipBoots(const USkeletalMesh* ItemMesh);
	
	// 나중에 무기 장착 해제할때
	UFUNCTION(BlueprintImplementableEvent)
	void EquipWeapon(const USkeletalMesh* ItemMesh);

	//UFUNCTION(BlueprintImplementableEvent)
	//void EquipWeapon(const UStaticMesh* ItemMesh);


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> mSpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> mCameraComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> mWeaponStaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UEquipVisualComponent> mEquipVisualComp;
};

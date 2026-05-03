// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASSTUDY_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// TargetLock 근데 이건 플레이어만 필요하긴한데.. 이게 진짜 필요한건지 생각해봐야함
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Combat|TargetLockComponent")
	virtual FVector GetLockPoint() { return FVector(); } 

	UFUNCTION(BlueprintCallable, Category = "Combat|TargetLockComponent")
	virtual void StopTargetLock(bool Value) {};

	// Attck 
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	virtual void SetAttackName(FName NewAttackName) = 0;

	//UFUNCTION(BlueprintCallable, Category = "Utils|Character")
	//virtual void SetMovementModeAndCollision(EMovementMode NewMovementMode, ECollisionEnabled::Type NewCollisionEnabled) = 0;

	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	virtual bool IsAttacking() = 0;

	//UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	//virtual void DoNextAttack() = 0;

	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	virtual bool IsBlocking() = 0;


	// HitStop
	UFUNCTION(BlueprintCallable, Category = "Combat|HitStop")
	virtual void StartHitStop(float Duration) {};

	UFUNCTION(BlueprintCallable, Category = "Combat|HitStop")
	virtual void EndHitStop() {};


	// Trace 
	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponTraceComponent")
	virtual bool SetWeaponTraceInfo(FName SocketStart, FName SocketEnd, int32 AnimIdx, USkeletalMeshComponent* MeshComp, float NotifyStartTime, EWeaponTraceShape TraceShape) { return false; };

	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponTraceComponent")
	virtual void Trace() {};

	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponTraceComponent")
	virtual void WeaponTraceReset() {};

	UFUNCTION(BlueprintCallable, Category = "Combat|WeaponTraceComponent")
	virtual bool GetWeaponTraceActivate() { return false; }
	
	// Target 
	// 플레이어와 적 캐릭터가 각각 상속받아서 
	// 플레이어는 Target Lock Component 의 타겟을
	// 적은 BlackBoard 에서 Target 을 반환하도록 구현하면 됨 
	UFUNCTION(BlueprintCallable, Category = "Combat|Character")
	virtual AActor* GetTarget() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Combat|Character")
	virtual void RotateToTarget() {};

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Combat|Character")
	virtual void SetStrafe(bool Value) = 0;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Combat|Character")
	virtual void SetAim(bool Value) {}
};

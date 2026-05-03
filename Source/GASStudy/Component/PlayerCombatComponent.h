// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Component/CombatComponent.h"
#include "Shared/Commbat/CombatData.h"
#include "PlayerCombatComponent.generated.h"




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GASSTUDY_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Attack();

	void AddCombatInput(int32 InputID);

	void ActivateCombatCancle(bool bActivate);

	void ConsumeInput();

	bool ActivateAbilityByInputID(int32 InputID);


private:
	/**
	* 슬로우 모션 
	*/
	void SetWorldTimeDilation(float Value);

public:
	void HandleInputTimer();

private:
	// 입력 버퍼 

	// UPROPERTY 를 달면 에러가 뜬다
	UPROPERTY()
	TArray<FCombatInputData> mCombatInputBuffer;

private:
	bool bCanCombatCancle = false;
	
	TOptional<int32> mPendingInputID;

	bool bCancelWindowOpen = false;

	float InputBufferTime = 1.f; 

	FTimerHandle mInputTimerHandle;
};

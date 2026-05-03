// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Character/NonPlayable/NonPlayableCharacter.h"
#include "AI/Data/AIAttackData.h"
#include "Enemy.generated.h"


class UEnemyCombatComponent;
class AWeaponBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDead, class AEnemy*);

UCLASS()
class GASSTUDY_API AEnemy 
	: public ANonPlayableCharacter
{
	GENERATED_BODY()
	
public:
	AEnemy();
	
public:
	virtual void BeginPlay() override;
	
protected:
	virtual void PostInitializeComponents() override;
	
public:
	FORCEINLINE UEnemyCombatComponent* GetCombatComponent() const
	{
		return mEnemyCombatComp;
	}

	void SetStrafe(bool Value);
	void SpawnWeapon();
	AWeaponBase* GetWeapon() const
	{
		return mCurrentEquipWeapon;
	}

protected:
	virtual void EventDeath(const FGameplayTag Tag, int32 NewCount) override;


#pragma region 레거시_전투
	// UtilInterface override
	// EnemyInterface 
	//virtual bool DoAttack(EnemyType SelectedAttackType) override;
	//virtual bool DoBlock() override;
	//virtual void EndBlock() override;
	//virtual void AddDamagedCount() override;
	//virtual int32 GetDamagedCount() override;
	//virtual void RestDamagedCount() override;
	//virtual bool CheckCanCounter() override;
	//virtual bool DoCounter(ECounterType CounterType) override;
	//virtual void EndCounter() override; 
	//virtual bool DoDodge() override;
	//virtual bool CanFinishCounter() override;
	//virtual void SetEnemyGait(EGait NewGait) override;
	//virtual bool DoSpecialAttack() override;

	//// CombatInterface override
	//// return FVector();
	//virtual FVector GetLockPoint() override;
	//virtual void SetAttackName(FName NewAttackName) override;
	//virtual bool IsAttacking() override;
	//virtual void DoNextAttack() override;
	//virtual bool IsBlocking() override;
	//virtual void ComboInputSwitch(class UANS_ComboAttack* ANS, bool Value) override;
	//virtual void StartHitStop(float Duration) override;
	//virtual void EndHitStop() override;
	//virtual bool SetWeaponTraceInfo(FName SocketStart, FName SocketEnd, int32 AnimIdx, USkeletalMeshComponent* MeshComp, float NotifyStartTime, EWeaponTraceShape TraceShape);
	//virtual void Trace() override;
	//virtual void WeaponTraceReset() override;
	//virtual AActor* GetTarget() const override;
	//virtual void SetStrafe(bool Value) override;
	//virtual void RotateToTarget() override;

//protected:
//
//	void SetCanNextCombo(bool CanCombo);
//	const FAIAttackTableInfo* SelectAttack(EnemyType SelectedAttackType);
//	void SetCounterType(ECounterType NewType) { CurrentCounterType = NewType; }
//	UFUNCTION()
//	void SetEnemyInfoToHUD(const FGameplayTag Tag, int32 NewCount);
//
//	UFUNCTION()
//	void DeadMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted);
//
//	UPROPERTY(BlueprintReadOnly)
//	FName CurrentAttackName;
//
//	FAIAttackTableInfo* CurrentAttackData;
//	FName CurrentDataTableName;
//
//	int32 DamagedCount = 0;
//
//
//	bool BCanBlock = false;
//
//	ECounterType CurrentCounterType = ECounterType::None;
//
//	// 타겟
//	UPROPERTY()
//	TWeakObjectPtr<AActor> CurrentTargetActor;
//
//	FDelegateHandle TagAddHandle;
//
//public:
//	// 현재 퍼블릭으로 만들어놨지만 추후 프렌드 선언으로 변경이 필요해보인다.
	FOnEnemyDead OnEnemyDead;
#pragma endregion 레거시_전투

public:

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyAttributeSet> mEnemyAttributeSet;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UMotionWarpingComponent> mMotionWarpingComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UEnemyCombatComponent> mEnemyCombatComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UQuestTargetComponent> mQuestTargetComp;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (DisplayName = "WeaponClass"))
	TSubclassOf<class AWeaponBase> mWeaponClass;

	UPROPERTY()
	TObjectPtr<AWeaponBase> mCurrentEquipWeapon;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UWeaponTraceComponent> mWeaponTraceComp;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/Commbat/CombatData.h"
#include "CombatComponent.generated.h"

struct FGameplayEventData;

UENUM(BlueprintType)
enum class EAttackResult : uint8
{
	Hit         UMETA(DisplayName = "피격"),
	Block       UMETA(DisplayName = "방어"),
	Parry       UMETA(DisplayName = "패링"),
};


struct FGASAttackAbilityData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	EAttackResult HandleIncomingAttack(AActor* Attacker, const FGameplayEventData& EventData);
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentBlockPosition(EBlockPosition NewPosition);

	UFUNCTION(BlueprintCallable)
	EBlockPosition GetCurrentBlockPosition() const;

	void SetAttackDataRow(FGASAttackAbilityData* AttackDataRow)
	{
		mAttackDataRow = AttackDataRow;
	}

	void SetMontageLevel(int Level)
	{
		mMontageLevel = Level;
	}

	const FGASAttackAbilityData* GetAttackDataRow() const
	{
		return mAttackDataRow;
	}

	int32 GetMontageLevel() const
	{
		return mMontageLevel;
	}

protected:
	EAttackResult ResolveAttackResult(AActor* Attacker);

	void HandleParry(AActor* Attacker, const FGameplayEventData& EventData);
	void HandleBlock(AActor* Attacker, const FGameplayEventData& EventData);
	void HandleHit(AActor* Attacker, const FGameplayEventData& EventData);

protected:
	UPROPERTY()
	TWeakObjectPtr<class UAbilitySystemComponent> mASC;
	
	// 블럭모션 왼쪽/오른쪽 
	UPROPERTY(BlueprintReadOnly)
	EBlockPosition mCurrentBlockPosition = EBlockPosition::Right;

private:
	FGASAttackAbilityData* mAttackDataRow;
	int32 mMontageLevel;
};

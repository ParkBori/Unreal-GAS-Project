// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Perception/AIPerceptionTypes.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Shared/Weapon/WeaponData.h"
#include "GASData.generated.h"

/**
 * 
 */

 // [Struct] FGASSecondaryEffectData
 // Desc: 공격 시 부가 GE 적용 정보
 // Usage: 
USTRUCT(BlueprintType)
struct FGASSecondaryEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SecondaryEffectClass", ToolTip = "부가 GE 클래스"))
	TSubclassOf<UGameplayEffect> SecondaryEffectClass = nullptr;

	UPROPERTY(Category = "Param", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Magnitude", ToolTip = "부가 GE 크기 설정 값"))
	float Magnitude = 0.f;
};

class UGE_Damage;

// [Struct] FGASAttackData
// Desc: 특정 스킬 내의 하나의 공격 정보
// Usage: 
USTRUCT(BlueprintType)
struct FGASAttackData
{
	GENERATED_BODY()

public:
	// 커브 데미지 데이터 -> 콤보에 따라서 변화되는 데미지 계수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DamageData"))
	FScalableFloat DamageData;

	// 커브 넉백 데이터 -> 콤보에 따라서
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Knockback"))
	FScalableFloat Knockback;

	// 공격시 -> 화상 / 출혈 / 포이즌을 타겟에게 부여
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Debuff"))
	FGASSecondaryEffectData Debuff;
};

// [Struct] FGASAttackAbilityData
// Desc: 특정 스킬 정보 (DT Row)
// Usage: 
USTRUCT(BlueprintType)
struct FGASAttackAbilityData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Attack", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDatas", ToolTip = "공격 정보들"))
	TArray<FGASAttackData> AttackDatas;
};

/**
* GAS 컴포넌트 사용 캐릭터 초기화 데이터
*/
USTRUCT(BlueprintType)
struct FGASLevelData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DefaultEffects", ToolTip = "초기 추가 적용 GE"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
};

/** Target data with just a source and target location in space */
USTRUCT(BlueprintType)
struct GASSTUDY_API FGameplayAbilityTargetData_WeaponTraceData : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	/** Generic location data for source */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponTrace)
	FName mStartSocket;

	/** Generic location data for target */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponTrace)
	FName mEndSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponTrace)
	FName mAnimName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponTrace)
	float mNotifyStartTime = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponTrace)
	EWeaponTraceShape mWeaponTraceShape = EWeaponTraceShape::None;

	// -------------------------------------
	FGameplayAbilityTargetData_WeaponTraceData() {}
	
	FGameplayAbilityTargetData_WeaponTraceData(FName StartSocket, FName EndSocket, FName AnimName, float NotifyStartTime, EWeaponTraceShape WeaponTraceShape)
		: mStartSocket(StartSocket)
		, mEndSocket(EndSocket)
		, mAnimName(AnimName)
		, mNotifyStartTime(NotifyStartTime)
		, mWeaponTraceShape(WeaponTraceShape)
	{

	}

	// -------------------------------------

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_WeaponTraceData::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FGameplayAbilityTargetData_WeaponTraceData");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;
		return true;
	}

};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_WeaponTraceData> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_WeaponTraceData>
{
	enum
	{
		WithNetSerializer = true,
	};
};

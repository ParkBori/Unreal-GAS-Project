// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterInfo.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Shared/Macro/Log.h"
#include "Shared/Commbat/CombatData.h"
#include "RPGCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


namespace  ECharacterOutlineType
{
	enum Type : uint8
	{
		None,
		Critical = 2,
		SuperArmor = 3,
		Groggy = 4,
	};
}



DECLARE_MULTICAST_DELEGATE(FOnDead);

UCLASS()
class GASSTUDY_API ARPGCharacter 
	: public ACharacter
	, public IAbilitySystemInterface
	, public IGenericTeamAgentInterface
	, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team)
	TEnumAsByte<EFraction> Fraction = EFraction::Civilians;

	FGenericTeamId TeamID;

	// Generic Interface 
	virtual FGenericTeamId GetGenericTeamId() const override;

	// AbilitySystemComponent 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// IGameplayTagAssetInterface  
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

public:
	void SetMovementModeAndCollision(EMovementMode NewMovementMode, ECollisionEnabled::Type NewCollisionEnabled);

	
protected:
	/**
	* 어빌리티시스템 태그 기반 델리게이트 전체 처리
	* 이함수 내부에 작성
	* 
	* ex) EventDeath 는 죽은 상태의 태그에 바인됭 이벤트인데, 해당 함수를 이 함수 내부에서 바인딩해주면 된다
	*/
	virtual void BindDelegateToAbilitySystem();

	/**
	* 태그에 바인딩 되는 함수
	* 플레이어 캐릭터가 죽었을때 일어나는 이벤트 처리
	* 자식 클래스에서 재정의가 필요함
	*/
	virtual void EventDeath(const FGameplayTag Tag, int32 NewCount);


public:
	// Character Movement 
	UFUNCTION(BlueprintCallable)
	void SetGait(EGait NewGait);

	UFUNCTION(BlueprintCallable)
	void SetStance(EStance NewStance);

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterSpeed(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	EGait GetGait() { return CurrentGait; }

protected:
	/* Try to activate all gameplay abilities with that match tag.*/
	/* 태그와 일치하는 어빌리티가 있다면 어빌리티 실행*/
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool ActivateAbilityWithTags(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation = true);

	/* Called When Character LevelUp */
	virtual void ApplyDefaultsGE();

	virtual void RemoveDefaultAttributeEffects();

	virtual void SetTestAbilites();

	virtual void SetMeleeAttackAbility();

	UFUNCTION(BlueprintCallable, Category = "Abilities|Melee", meta = (DeprecatedFunction, DeprecatedMessage = "Use ActivateAbilityBySlot instead."))
	bool ActivateMeleeAttackAbility(bool AllowRemoteActivate = true);

	/* 현재 실행중인 어빌리티들의 태그를 ActiveAbilities 에 담는다. */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void GetActiveAbilitiesWitTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ApplyGameplayEffect(TSubclassOf<class UGameplayEffect> GameplayEffect);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool CanApplyGameplayEffect(TSubclassOf<class UGameplayEffect> GameplayEffect);

	//UFUNCTION(BlueprintCallable, Category = "Abilities")
	//virtual bool EquipWeapon(ARPGWeapon* Weapon, TEnumAsByte<EWeaponSlot> EquipSlot);
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool ActivateAbilityBySlot(TEnumAsByte<EAbilitySlot> AbilitySlot, bool AllowRemoteActivate = true);

	virtual void ClearAbilitySlot(TEnumAsByte<EAbilitySlot> AbilitySlot);
	virtual void AddAbilitySlot(TSubclassOf<UGameplayAbility> NewAbility, TEnumAsByte<EAbilitySlot> AbilitySlot);
	
	//virtual void EquipRightHand(ARPGWeapon* Weapon);
	//virtual void EquipLeftHand(ARPGWeapon* Weapon);
	//virtual void EquipTwoHand(ARPGWeapon* Weapon);

	//UFUNCTION(BlueprintCallable)
	//void DetectEnemy();

	//UFUNCTION(BlueprintCallable)
	//void DetectEnemyHandler();

protected:
	/*
	* 캐릭터의 패링 불가능한 공격 / 상태등
	* 시각적으로 표현하기 위해 아웃라이너를 적용시킴
	*/
	void ChangeOutline(ECharacterOutlineType::Type OutlineType);

private:
	void Callback_AnyTagChanged(const FGameplayTag Tag, int32 NewCount);

public:
	// TODO - CombatComponent 에 선언되어서 추후 이쪽 정리하고, 컴뱃쪽에서 접근하도록 수정 
	UPROPERTY(BlueprintReadOnly)
	EBlockPosition CurrentBlockPosition = EBlockPosition::Left;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EStance GetStance() { return CurrentStance; }

	int32 GetCharacterLevel()
	{
		return CharacterLevel;
	}

	UFUNCTION(BlueprintCallable)
	const FName& GetCharacterName() const
	{
		return CharacterName;
	}

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDefaultAttributeSet> mDefaultAttributeSet;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UTimelineComponent> SlowmotionTimelineComponent;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UCurveFloat> SlowmotionCurve;

	/* 캐릭터 레벨*/
	// 캐릭터 레벨로 스텟 초기화
	UPROPERTY(EditAnywhere, Category = "Attributes|Level")
	int32 CharacterLevel;

	/* 캐릭터 이름*/
	// 캐릭터 이름으로 GE 초기화
	UPROPERTY(EditAnywhere, Category = "Attributes|CharacterName")
	FName CharacterName;
	
	/* Set this to true if you want to add test abilities to character's ability system component. */
	UPROPERTY(EditAnywhere, Category = "Attributes|Debug")
	bool EnableTestAbilites = false;

	UPROPERTY(EditAnywhere, Category = "Attributes|Debug")
	TArray<TSubclassOf<class UGameplayAbility>> TestAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities|Melee")
	TSubclassOf<class UGameplayAbility> MeleeAttackAbility;

	UPROPERTY()
	FGameplayAbilitySpecHandle MeleeAttackAbilitySpecHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Attack|Finisher")
	bool CanDoFinisher;

	UPROPERTY(BlueprintReadWrite, Category = "Attack|Slowmotion")
	bool DisableSlowmotion;

	TMap<TEnumAsByte<EAbilitySlot>, FGameplayAbilitySpecHandle> SlotAbilityHandles;


public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WantSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SpeedInterpSpeed = 10.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EGait CurrentGait = EGait::Jog;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EStance CurrentStance = EStance::Normal;


public:
	FOnDead mOnDead;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AttributeSetDefault.generated.h"



#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



static float AdjustRatio(float Current, float OldMax, float NewMax)
{
	// 분모가 0인 경우 방어 로직 추가 (안전성)
	if (OldMax <= 0.f)
	{
		return NewMax;
	}

	float Ratio = Current / OldMax;
	float NewCalculated = Ratio * NewMax;

	return FMath::Clamp(NewCalculated, 0.0f, NewMax);
}

/*
*/
UCLASS()
class GASSTUDY_API UDefaultAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;



	//void AdjustAttributeForMaxChange(
	//	FGameplayAttributeData& AffectedAttribute
	//	, const FGameplayAttributeData& MaxAttribute
	//	, float NewMaxValue
	//	, const FGameplayAttribute& AffectedAttributeProperty);



public:
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Defense", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;
	

public:
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Defense);
};

UCLASS()
class GASSTUDY_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;


private:
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "MaxStamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

public:
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina);
};

UCLASS()
class GASSTUDY_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;



private:
	UPROPERTY(BlueprintReadOnly, Category = "Groggy", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Groggy;

	UPROPERTY(BlueprintReadOnly, Category = "Groggy", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxGroggy;

public:
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, Groggy);
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, MaxGroggy);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "SpawnActorAttachGEComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UENUM(BlueprintType)
enum class ESpawnTransformType : uint8
{
	None,
	Source,
	Target
};

USTRUCT(BlueprintType)
struct FSpawnAttachedNiagaraParameters
{
	GENERATED_BODY()

	UPROPERTY(Category = "Spawn Parameter", EditDefaultsOnly, meta = (DisplayName = "NiagaraAsset"))
	TSoftObjectPtr<UNiagaraSystem> mNiagaraAsset = nullptr;
	
	UPROPERTY(Category = "Spawn Parameter", EditDefaultsOnly, meta = (DisplayName = "TransformType"))
	ESpawnTransformType mTransformType = ESpawnTransformType::None;

	UPROPERTY(Category = "Attach Parameter", EditDefaultsOnly, meta = (DisplayName = "HasToAttach"))
	bool mHasToAttach = false;
	
	FAttachmentTransformRules mAttachType;
	
	UPROPERTY(Category = "Attach Parameter", EditDefaultsOnly, meta = (DisplayName = "SocketName"))
	FName mSocketName;

	FSpawnAttachedNiagaraParameters();

	bool IsValid() const;
};

/**
* 
*/
UCLASS(DisplayName = "Spawn Niagara Attach")
class GASSTUDY_API USpawnNiagaraAttachGEComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	USpawnNiagaraAttachGEComponent();

public:
	// 이펙트가 적용될 때 호출
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const override;

public:
	void SetParameters(const FSpawnAttachedNiagaraParameters& Parameters);

protected:
	FSpawnAttachedNiagaraParameters mParameters;
};
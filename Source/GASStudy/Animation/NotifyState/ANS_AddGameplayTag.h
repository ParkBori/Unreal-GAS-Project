// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_AddGameplayTag.generated.h"


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player			UMETA(DisplayName = "Player"),
	NonPlayer		UMETA(DisplayName = "NonPlayer"),
	All				UMETA(DisplayName = "All"),
};

/**
 * 플레이어, 적, 혹은 GAS를 소유하는 모든 액터에게 태그 부여
 */
UCLASS()
class GASSTUDY_API UANS_AddGameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	ECharacterType mAddTagCharacterType;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer mAddTags;

	UPROPERTY()
	TWeakObjectPtr<class UAbilitySystemComponent> mASC = nullptr;
};

/**
* 모든 GAS 소유 액터에게 State::State_CancelWindow를 부여하는 태그
*/
UCLASS()
class GASSTUDY_API UANS_CancelWindow : public UANS_AddGameplayTag
{
	GENERATED_BODY()

public:
	UANS_CancelWindow();

};

/**
* AI가 타겟이 공격을 하고있는 것을 인지하는 타이밍
*/
UCLASS()
class GASSTUDY_API UANS_AttackActivate : public UANS_AddGameplayTag
{
	GENERATED_BODY()

public:
	UANS_AttackActivate();
};


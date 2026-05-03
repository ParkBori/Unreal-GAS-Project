// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_GATrigger.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UAN_GATrigger : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAN_GATrigger();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere, Category = GAS)
	float AbilityAEndTime;
};

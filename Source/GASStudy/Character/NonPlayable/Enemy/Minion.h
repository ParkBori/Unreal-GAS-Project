// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/NonPlayable/Enemy/Enemy.h"
#include "Minion.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API AMinion : public AEnemy
{
	GENERATED_BODY()
	
public:
	AMinion();

protected:
	virtual void BeginPlay() override;
	virtual void EventDeath(const FGameplayTag Tag, int32 NewCount) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void InteractQuest();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface>		mWidgetMaterial;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess))
	TObjectPtr<class UGASWidgetComponent> mGASWidgetComp;
};

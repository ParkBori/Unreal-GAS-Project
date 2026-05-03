// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPCController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyController.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetActorChanged, AActor* /*TargetActor*/);

/**
 * 
 */
UCLASS()
class GASSTUDY_API AEnemyController : public ANPCController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

	void StopPerception();


protected:
	void OnPossess(APawn* aPawn);

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Callback_OnTarget(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionComponent> mAIPerceptionComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAISenseConfig_Sight> mSightConfig;

private:
	FOnTargetActorChanged mOnTargetActorChanged;
};

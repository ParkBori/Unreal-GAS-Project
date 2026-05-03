// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Shared/WorldTransition/WorldTransitionData.h"
#include "WorldTransitionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API UWorldTransitionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	* 로딩 레벨로 전환
	*/
	UFUNCTION(BlueprintCallable)
	void RequestTransition(const FTransitionRequest& Request);

public:
	const FTransitionRequest& GetPendingRequest() const { return mTransitionRequest; }

private:
	FTransitionRequest mTransitionRequest;
	void HandlePostLoadMap(UWorld* LoadedWorld);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Shared/Commbat/CombatData.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */



UCLASS()
class GASSTUDY_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static EHitDirection CalculateDirection(const AActor* StandardActor, const AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	static EHitDirectionArea CalculateDirectionToLocation(const AActor* StandardActor, const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable)
	static EHitDirectionArea CalculateDirectionToHitResult(const AActor* StandardActor, const FHitResult& HitResult);


	//UFUNCTION(BlueprintPure)
	//static FGameplayAbilityTargetDataHandle MakeTargetDataHandleforAnimation(const FGameplayAbilityTargetData_AnimationInfo& Info);

	//UFUNCTION(BlueprintPure)
	//static FGameplayAbilityTargetData_Animation GetTargetDataAnimation(const FGameplayAbilityTargetDataHandle& Handle, int32 Index);

	//UFUNCTION(BlueprintPure)
	//static FGameplayAbilityTargetDataHandle MakeTargetDataHandleforEnemyAttack(const FGameplayAbilityTargetData_EnemyAttackInfo& Info);

	//UFUNCTION(BlueprintPure)
	//static FGameplayAbilityTargetData_EnemyAttack GetTargetDataEnemyAttack(const FGameplayAbilityTargetDataHandle& Handle, int32 Index);
};

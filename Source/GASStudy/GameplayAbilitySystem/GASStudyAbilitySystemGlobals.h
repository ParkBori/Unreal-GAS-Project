// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GASStudyAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class GASSTUDY_API UGASStudyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	static UGASStudyAbilitySystemGlobals& Get()
	{
		return *Cast<UGASStudyAbilitySystemGlobals>(IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals());
	}

	virtual void InitGlobalData() override;

public:
	UDataTable* GetGlobalSkillDataTable();
	UDataTable* GetDefaultAbilityDataTable();

protected:
	UPROPERTY()
	TObjectPtr<UDataTable> mGlobalSkillDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> mDefaultAbilityDataTable;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AbilitiesDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "GASStudy Gameplay Abilities Settings"))
class GASSTUDY_API UAbilitiesDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = Skill, meta = (DisplayName = "Global Skill Data Table", AllowedClasses = "/Script/Engine.DataTable", ConfigRestartRequired = true))
	FSoftObjectPath mGlobalSkillDataTableName;

	UPROPERTY(Config, EditDefaultsOnly, Category = Skill, meta = (DisplayName = "Default Ability Data Table", AllowedClasses = "/Script/Engine.DataTable", ConfigRestartRequired = true))
	FSoftObjectPath mDefaultAbilityDataTableName;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GASStudyAbilitySystemGlobals.h"
#include "DeveloperSettings/AbilitiesDeveloperSettings.h"

void UGASStudyAbilitySystemGlobals::InitGlobalData()
{
	Super::InitGlobalData();
	GetGlobalSkillDataTable();
}

UDataTable* UGASStudyAbilitySystemGlobals::GetGlobalSkillDataTable()
{
	if (mGlobalSkillDataTable == nullptr)
	{
		const UAbilitiesDeveloperSettings* DeveloperSettings = GetDefault<UAbilitiesDeveloperSettings>();
		if (DeveloperSettings->mGlobalSkillDataTableName.IsValid())
		{
			GlobalAttributeMetaDataTable = Cast<UDataTable>(DeveloperSettings->mGlobalSkillDataTableName.TryLoad());
		}
	}
	return GlobalAttributeMetaDataTable;
}

UDataTable* UGASStudyAbilitySystemGlobals::GetDefaultAbilityDataTable()
{
	if (mDefaultAbilityDataTable == nullptr)
	{
		const auto* DeveloperSettings = GetDefault<UAbilitiesDeveloperSettings>();
		if (DeveloperSettings->mDefaultAbilityDataTableName.IsValid() == true)
		{
			mDefaultAbilityDataTable = Cast<UDataTable>(DeveloperSettings->mDefaultAbilityDataTableName.TryLoad());
		}
	}
	return mDefaultAbilityDataTable;
}

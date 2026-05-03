// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/GameDataSettings.h"

UDataTable* UGameDataSettings::GetDataTable(EGlobalGameDataTableType TableType) const
{
	if (const TSoftObjectPtr<UDataTable>* TablePtr = mDataTables.Find(TableType))
	{
		return TablePtr->LoadSynchronous();
	}
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSetting/CustomWorldSettingBase.h"

UDataTable* ACustomWorldSettingBase::GetDataTable(EWorldDataTableType TableType) const
{
	auto DataTablePtr = mDataTables.Find(TableType);
	return DataTablePtr == nullptr ? nullptr : *DataTablePtr;
}

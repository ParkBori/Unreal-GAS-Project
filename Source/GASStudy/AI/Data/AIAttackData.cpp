// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Data/AIAttackData.h"

//UAIAttackData::UAIAttackData()
//{
//	static ConstructorHelpers::FObjectFinder<UDataTable> GreateSwordDataTableAsset(TEXT(""));
//	if (GreateSwordDataTableAsset.Succeeded())
//	{
//		GreateSwordAttackDataTable = GreateSwordDataTableAsset.Object;
//	}
//
//	static ConstructorHelpers::FObjectFinder<UDataTable> KatanDataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Character/Final/NonPlayableCharacter/DT_AIKatanaAttack.DT_AIKatanaAttack'"));
//	if (KatanDataTableAsset.Succeeded())
//	{
//		KatanaAttackDataTable = KatanDataTableAsset.Object;
//	}
//
//	static ConstructorHelpers::FObjectFinder<UDataTable> KatanSpecailDataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Character/Final/NonPlayableCharacter/DT_AIKatanaSpecialAttack.DT_AIKatanaSpecialAttack'"));
//	if (KatanSpecailDataTableAsset.Succeeded())
//	{
//		KatanaSpecailAttackDataTable = KatanSpecailDataTableAsset.Object;
//	}
//
//	static ConstructorHelpers::FObjectFinder<UDataTable> KatanCounterDataTableAsset(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Character/Final/NonPlayableCharacter/DT_AIKatanaCounterAttack.DT_AIKatanaCounterAttack'"));
//	if (KatanCounterDataTableAsset.Succeeded())
//	{
//		KatanaCounterAttackDataTable = KatanCounterDataTableAsset.Object;
//	}
//}
//
//FAIAttackTableInfo* UAIAttackData::FindAIAttackInfo(const FName& FindName, EnemyType Type) const
//{
//	switch (Type)
//	{
//	case EnemyType::GreateSword:
//		return GreateSwordAttackDataTable->FindRow<FAIAttackTableInfo>(FindName, TEXT(""));
//
//	case EnemyType::Katana:
//		return KatanaAttackDataTable->FindRow<FAIAttackTableInfo>(FindName, TEXT(""));
//	
//	case EnemyType::KatanaSpecial:
//		return KatanaSpecailAttackDataTable->FindRow<FAIAttackTableInfo>(FindName, TEXT(""));
//	
//	case EnemyType::KatanaCounter:
//		return KatanaCounterAttackDataTable->FindRow<FAIAttackTableInfo>(FindName, TEXT(""));
//	}
//
//	return nullptr;
//}
//
//int32 UAIAttackData::GetDataTableRowCount(EnemyType Type) const
//{
//	int RowCount = 0;
//
//	switch (Type)
//	{
//	case EnemyType::GreateSword:
//		RowCount = GreateSwordAttackDataTable->GetRowMap().Num();
//		break;
//	case EnemyType::Katana:
//		RowCount = KatanaAttackDataTable->GetRowMap().Num();
//		break;
//	case EnemyType::KatanaSpecial:
//		RowCount = KatanaSpecailAttackDataTable->GetRowMap().Num();
//		break;
//	case EnemyType::KatanaCounter:
//		RowCount = KatanaCounterAttackDataTable->GetRowMap().Num();
//		break;
//	}
//
//	return RowCount;
//}

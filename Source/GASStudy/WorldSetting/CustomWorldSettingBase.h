// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "CustomWorldSettingBase.generated.h"

/**
 * 
 */

class AInventoryCharacter;

 // [Enum] EDataTableType
 // Desc: 데이터 테이블 타입
 // Usage: AEXWorldSettings 내부 DataTable 탐색 시, Key 값
UENUM(BlueprintType)
enum class EWorldDataTableType : uint8
{
	EnemyAIData			UMETA(ToolTip = "몬스터 캐릭터 AI 기본 정보"),
	QuestData	UMETA(ToolTip = "메인 스토리 퀘스트"),
};


UCLASS()
class GASSTUDY_API ACustomWorldSettingBase : public AWorldSettings
{
	GENERATED_BODY()

public:
	/**
	 * WorldSetting에서 원하는 Data Table을 찾기 위한 함수
	 * @param TableType 데이터 테이블 종류
	 */
	UDataTable* GetDataTable(EWorldDataTableType  TableType) const;

	/**
	 * WorldSetting에서 원하는 Data Table Row를 찾기 위한 함수
	 *
	 * @tparam 반환 TableRow타입
	 * @param TableType 데이터 테이블 종류
	 * @param Key 테이블 내 Row를 찾기 위한 Key 값
	 * @return T*로 캐스팅한 FTableRowBase* 값
	 */
	template<typename T>
	T* GetDataTableRow(EWorldDataTableType  TableType, const FName& Key) const;

protected:
	/**
	 * 해당 월드에서 사용할 DataTable들 (월드에 종속적임)
	 */
	UPROPERTY(Category = Data, EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "DataTables"))
	TMap<EWorldDataTableType, TObjectPtr<UDataTable>> mDataTables;
};

template<typename T>
inline T* ACustomWorldSettingBase::GetDataTableRow(EWorldDataTableType  TableType, const FName& Key) const
{
	if (mDataTables.Find(TableType) != nullptr)
	{
		return mDataTables[TableType]->FindRow<T>(Key, TEXT(""));
	}
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameDataSettings.generated.h"

/**
 * 
 */

// [Enum] EDataTableType
// Desc: 데이터 테이블 타입
// Usage: AEXWorldSettings 내부 DataTable 탐색 시, Key 값
UENUM(BlueprintType)
enum class EGlobalGameDataTableType : uint8
{
	ItemData			UMETA(ToolTip = "아이템 데이터"),
	QuestTriggerData	UMETA(ToolTip = "퀘스트 트리거 데이터"),
	QuestData			UMETA(ToolTip = "게임 전체 퀘스트"),
	LinkAnimData		UMETA(ToolTip = "태그기반 애니메이션 데이터"),
	ShopNPCItemDat		UMETA(ToolTip = "상점 NPC 판매 아이템 목록"),
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Game Data Settings"))
class GASSTUDY_API UGameDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	/**
	* 여기서 이런식의 접근을 사용하는 이유
	* 
	* mDataTables 을 얻고자 해당 클래스에 접근
	* 하지만, 본클래스를 static 으로 해버리면, 멤버 변수인 인스턴스에 접근이 불가능
	* 따라서 아래와 같이 인스턴스를 반환하고, 해당 함수를 통해 데이터테이블에 접근
	*/
	static const UGameDataSettings* Get()
	{
		return GetDefault<UGameDataSettings>();
	}

	/**
	 * WorldSetting에서 원하는 Data Table을 찾기 위한 함수
	 * @param TableType 데이터 테이블 종류
	 */
	UDataTable* GetDataTable(EGlobalGameDataTableType  TableType) const;

	/**
	 * WorldSetting에서 원하는 Data Table Row를 찾기 위한 함수
	 *
	 * @tparam 반환 TableRow타입
	 * @param TableType 데이터 테이블 종류
	 * @param Key 테이블 내 Row를 찾기 위한 Key 값
	 * @return T*로 캐스팅한 FTableRowBase* 값
	 */
	template<typename T>
	T* GetDataTableRow(EGlobalGameDataTableType  TableType, const FName& Key) const;

private:
	/**
	 * 해당 월드에서 사용할 DataTable들
	 */
	UPROPERTY(EditAnywhere, Config, Category = "DataTables")
	TMap<EGlobalGameDataTableType, TSoftObjectPtr<UDataTable>> mDataTables;
};


template<typename T>
inline T* UGameDataSettings::GetDataTableRow(EGlobalGameDataTableType  TableType, const FName& Key) const
{
	if (UDataTable* Table = GetDataTable(TableType))
	{
		return Table->FindRow<T>(Key, TEXT("GameDataSettings"));
	}
	return nullptr;
}
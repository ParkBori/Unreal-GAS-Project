// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

/**
 *
 */

constexpr float DEFAULT_SOUND_VALUE = 70.f;

class UInventoryComponent_V2;
class UEquipComponent;
class UQuestComponent;

class UMetaSaveGame;
class UPlayerSaveGame;
class USounSaveGame;

DECLARE_MULTICAST_DELEGATE(FOnCompleteMetaSaveGameLoad);
DECLARE_DYNAMIC_DELEGATE(FOnSaveComplete);
DECLARE_DYNAMIC_DELEGATE(FOnLoadComplete);

UCLASS()
class GASSTUDY_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void SetDataByLoadedSaveGame();

public:
	UFUNCTION(BlueprintCallable)
	void SaveMetaSaveGame();

	UFUNCTION(BlueprintCallable)
	void SaveGame(const FOnSaveComplete& OnSaveComplete);

	void SaveGameSync();

	void SaveSound(const struct FSoundSaveData& SaveData);

public:
	UFUNCTION(BlueprintCallable)
	void LoadMetaSaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadMetaSaveGameSync();

	UFUNCTION(BlueprintCallable)
	void LoadGame(const FOnLoadComplete& OnLoadComplete);

	void LoadSound();

public:
	// ==== Util ====
	/**
	* @SaveSlotNames 존재하는 모든 세이브 파일이름을 넣어서 반환
	*/
	void GetSaveGameNames(TArray<FString>& SaveSlotNames);

	void GetSaveGameNames_V2(TArray<FString>& SaveSlotNames);

	/**
	* 마지막 저장 슬롯
	*/
	void SelectLastSavedGame();

	/**
	* 새 게임 선택시 실행
	*/
	void SelectNewGame();

	/**
	* 슬롯 이름기반 선택
	*/
	void SelectSavedGameBySlotName(const FName& SlotName);

	/**
	* 슬롯 이름기반 삭제
	*/
	void RequestRemoveSaveFile(const FName& SlotName);


	/**
	* @ SlotName 파일 이름
	* @ return 파일 이름에 해당하는 파일을 세이브 경로에서 찾아서, 마지막 수정시간을 가져옴 (UCT)
	*/
	FDateTime GetSaveFileTimestamp(const FString& SlotName);

	void GetSaveFileData(TMap<FString, FString>& SaveGameData);

	const FSoundSaveData GetSavedSoundData();

public:
	/**
	* 마지막 저장 포인트 위치
	*/
	const FTransform& GetLastSavePoint() const
	{
		return mLastSavePoint;
	}

private:
	/**
	* 플레이어 얻기
	*/
	AActor* GetCurrentPlayer() const;

	/**
	* 슬롯 이름 만들기
	*/
	FString MakeSlotName() const;

private:
	FTransform mLastSavePoint;

private:
	// 현재 세이브 파일 
	UPROPERTY()
	TObjectPtr<UPlayerSaveGame> mPlayerSaveGame;

	// 메타 세이브 게임
	UPROPERTY()
	TObjectPtr<UMetaSaveGame> mMetaSaveGame;

	UPROPERTY()
	TObjectPtr<USounSaveGame> mSoundSaveGame;

	FString mSelectedSlotName;

private:
	// 슬롯인덱스
	int32 mUserIdx = 0;
	static const FString SaveSlotName;
	static const FString SoundSlotName;

public:
	FOnCompleteMetaSaveGameLoad mOnCompleteMetaSaveGameLoad;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Subsystem/DialogueSubsystem.h"
#include "Subsystem/QuestSubsystem.h"


#include "Component/InventoryComponent_V2.h"
#include "Component/EquipComponent.h"
#include "Component/QuestComponent.h"

// SaveGame
#include "Save/MetaSaveGame/MetaSaveGame.h"
#include "Save/Player/PlayerSaveGame.h"
#include "Save/Sound/SounSaveGame.h"
#include "Shared/Macro/Log.h"

// 세이브 파일을 읽어오기 위한 헤더
#include "SaveGameSystem.h"
#include "PlatformFeatures.h"
#include "HAL/PlatformFileManager.h"

const FString USaveSubsystem::SaveSlotName = TEXT("backup");
const FString USaveSubsystem::SoundSlotName = TEXT("sound");

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// 메타 세이브 게임파일 로드
	LoadMetaSaveGameSync();
}

void USaveSubsystem::SetDataByLoadedSaveGame()
{
	AActor* Player = GetCurrentPlayer();
	if (!Player || !mPlayerSaveGame)
	{
		MY_LOG(Error, "");
		return;
	}

	UInventoryComponent_V2* IC = Player->FindComponentByClass<UInventoryComponent_V2>();
	UQuestComponent* QC = Player->FindComponentByClass<UQuestComponent>();
	UEquipComponent* EC = Player->FindComponentByClass<UEquipComponent>();

	if (!IC || !QC || !EC)
	{
		MY_LOG(Error, "LoadFail, Component null");
		return;
	}

	IC->LoadFromSaveData(mPlayerSaveGame);
	EC->LoadFromSaveData(mPlayerSaveGame);
	QC->LoadFromSaveData(mPlayerSaveGame);
}

void USaveSubsystem::SaveMetaSaveGame()
{
	if (!mMetaSaveGame)
	{
		return;
	}

	FMetaSaveData* MetaSaveData = mMetaSaveGame->mSavedSlotList.FindByPredicate([this](const FMetaSaveData& Other) {
		return Other.SlotName == mSelectedSlotName;
		});

	if (!mSelectedSlotName.IsEmpty())
	{
		if (MetaSaveData)
		{
			MetaSaveData->SaveDate = FDateTime::Now();
		}
		else
		{
			mMetaSaveGame->mSavedSlotList.Add(FMetaSaveData(mSelectedSlotName));
		}
	}

	UGameplayStatics::SaveGameToSlot(mMetaSaveGame, TEXT("MetaSaveGame"), 0);
}

void USaveSubsystem::SaveGame(const FOnSaveComplete& OnSaveComplete)
{
	if (!mPlayerSaveGame)
	{
		MY_LOG(Error, "SaveFail, SaveGame null");
		return;
	}

	AActor* Player = GetCurrentPlayer();

	UInventoryComponent_V2* IC = Player->FindComponentByClass<UInventoryComponent_V2>();
	UQuestComponent* QC = Player->FindComponentByClass<UQuestComponent>();
	UEquipComponent* EC = Player->FindComponentByClass<UEquipComponent>();


	if (!IC || !QC || !EC)
	{
		MY_LOG(Error, "LoadFail, Component null");
		return;
	}

	IC->MakeSaveData(mPlayerSaveGame);
	EC->MakeSaveData(mPlayerSaveGame);
	QC->MakeSaveData(mPlayerSaveGame);
	mPlayerSaveGame->SavePoint = Player->GetActorTransform();


	// HUD 에 데이터 저장 UI 생성후, SavedDelegate 바인딩해주면 저장완료되는 시점을 확인할 수 있음 
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindLambda([this, OnSaveComplete](const FString& SlotName, const int32 UserIndex, bool bSuccess) {
		MY_LOG(Warning, "%s", bSuccess ? TEXT("Save") : TEXT("Fail"));
		if (bSuccess)
		{
			SaveMetaSaveGame();
		}
		OnSaveComplete.ExecuteIfBound();
		});

	UGameplayStatics::AsyncSaveGameToSlot(mPlayerSaveGame, mSelectedSlotName, mUserIdx, SavedDelegate);
}

void USaveSubsystem::SaveGameSync()
{
	if (!mPlayerSaveGame)
	{
		MY_LOG(Error, "SaveFail, SaveGame null");
		return;
	}

	AActor* Player = GetCurrentPlayer();

	UInventoryComponent_V2* IC = Player->FindComponentByClass<UInventoryComponent_V2>();
	UQuestComponent* QC = Player->FindComponentByClass<UQuestComponent>();
	UEquipComponent* EC = Player->FindComponentByClass<UEquipComponent>();

	if (!IC || !QC || !EC)
	{
		MY_LOG(Error, "LoadFail, Component null");
		return;
	}

	IC->MakeSaveData(mPlayerSaveGame);
	EC->MakeSaveData(mPlayerSaveGame);
	QC->MakeSaveData(mPlayerSaveGame);
	mPlayerSaveGame->SavePoint = Player->GetActorTransform();

	UGameplayStatics::SaveGameToSlot(mPlayerSaveGame, mSelectedSlotName, mUserIdx);
	SaveMetaSaveGame();
}

void USaveSubsystem::SaveSound(const FSoundSaveData& SaveData)
{
	if (!mSoundSaveGame)
	{
		LoadSound();
	}

	if (!mSoundSaveGame)
	{
		return;
	}

	mSoundSaveGame->mSoundSaveData = SaveData;
	UGameplayStatics::SaveGameToSlot(mSoundSaveGame, SoundSlotName, mUserIdx);
}

void USaveSubsystem::LoadMetaSaveGame()
{
	// 메타세이브 파일 로드
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindLambda([this](const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData) {
		if (LoadedGameData)
		{
			mMetaSaveGame = Cast<UMetaSaveGame>(LoadedGameData);
		}
		else
		{
			mMetaSaveGame = Cast<UMetaSaveGame>(UGameplayStatics::CreateSaveGameObject(UMetaSaveGame::StaticClass()));
		}

		if (mOnCompleteMetaSaveGameLoad.IsBound())
		{
			mOnCompleteMetaSaveGameLoad.Broadcast();
		}
		});
	// USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
	// LoadedDelegate.BindUObject(SomeUObjectPointer, &USomeUObjectClass::LoadGameDelegateFunction);
	UGameplayStatics::AsyncLoadGameFromSlot(TEXT("MetaSaveGame"), mUserIdx, LoadedDelegate);
}

void USaveSubsystem::LoadMetaSaveGameSync()
{
	auto SaveData = UGameplayStatics::LoadGameFromSlot(TEXT("MetaSaveGame"), mUserIdx);
	if (IsValid(SaveData))
	{
		mMetaSaveGame = Cast<UMetaSaveGame>(SaveData);
	}
	else
	{
		mMetaSaveGame = Cast<UMetaSaveGame>(UGameplayStatics::CreateSaveGameObject(UMetaSaveGame::StaticClass()));
		SaveMetaSaveGame();
	}

	if (mOnCompleteMetaSaveGameLoad.IsBound())
	{
		mOnCompleteMetaSaveGameLoad.Broadcast();
	}
}

void USaveSubsystem::LoadGame(const FOnLoadComplete& OnLoadComplete)
{
	// Set up the delegate.
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;

	LoadedDelegate.BindLambda([this, OnLoadComplete](const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData) {
		if (LoadedGameData)
		{
			mPlayerSaveGame = Cast<UPlayerSaveGame>(LoadedGameData);
			mLastSavePoint = mPlayerSaveGame->SavePoint;
		}
		else
		{
			mPlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
		}
		SaveMetaSaveGame();

		OnLoadComplete.ExecuteIfBound();

		});
	UGameplayStatics::AsyncLoadGameFromSlot(mSelectedSlotName, mUserIdx, LoadedDelegate);
}

void USaveSubsystem::LoadSound()
{
	// Set up the delegate.
	// Retrieve and cast the USaveGame object to UMySaveGame.

	if (mSoundSaveGame = Cast<USounSaveGame>(UGameplayStatics::LoadGameFromSlot(SoundSlotName, 0)))
	{
		MY_LOG(Error, "Load Success");
	}
	else
	{
		mSoundSaveGame = Cast<USounSaveGame>(UGameplayStatics::CreateSaveGameObject(USounSaveGame::StaticClass()));
		mSoundSaveGame->mSoundSaveData = DEFAULT_SOUND_VALUE;
	}
}

void USaveSubsystem::GetSaveGameNames(TArray<FString>& SaveSlotNames)
{
	// 플랫폼 피처 모듈을 가져오기
	IPlatformFeaturesModule& PlatformFeatures = IPlatformFeaturesModule::Get();

	// 해당 모듈로부터 세이브 시스템 인터페이스가져오기
	ISaveGameSystem* SaveSystem = PlatformFeatures.GetSaveGameSystem();

	if (SaveSystem)
	{
		SaveSystem->GetSaveGameNames(SaveSlotNames, 0);
	}
}

void USaveSubsystem::GetSaveGameNames_V2(TArray<FString>& SaveSlotNames)
{
	if (!mMetaSaveGame)
	{
		MY_LOG(Error, "mMetaSaveGame");
		return;
	}

	SaveSlotNames.Empty();
	for (const auto& SaveSlot : mMetaSaveGame->mSavedSlotList)
	{
		SaveSlotNames.Add(SaveSlot.SlotName);
	}
}

void USaveSubsystem::SelectLastSavedGame()
{
	if (!mMetaSaveGame) return;

	if (mMetaSaveGame->mSavedSlotList.Num() == 0)
	{
		SelectNewGame();
		return;
	}

	// 시간 기반 오름차순 정렬
	mMetaSaveGame->mSavedSlotList.Sort(
		[](const FMetaSaveData& A, const FMetaSaveData& B)
		{
			return A.SaveDate > B.SaveDate;
		});

	// 가장 최근에 파일 선택
	mSelectedSlotName = mMetaSaveGame->mSavedSlotList[0].SlotName;
}

void USaveSubsystem::SelectNewGame()
{
	mSelectedSlotName = MakeSlotName();
}

void USaveSubsystem::SelectSavedGameBySlotName(const FName& SlotName)
{
	FMetaSaveData* Data = mMetaSaveGame->mSavedSlotList.FindByPredicate([SlotName](const FMetaSaveData& Other) {
		return Other.SlotName == SlotName;
		});

	if (Data && UGameplayStatics::DoesSaveGameExist(SlotName.ToString(), 0))
	{
		mSelectedSlotName = SlotName.ToString();
	}
}

void USaveSubsystem::RequestRemoveSaveFile(const FName& SlotName)
{
	FMetaSaveData* Data = mMetaSaveGame->mSavedSlotList.FindByPredicate([SlotName](const FMetaSaveData& Other) {
		return Other.SlotName == SlotName;
		});

	bool bDeleted = UGameplayStatics::DeleteGameInSlot(SlotName.ToString(), 0);
	if (Data && bDeleted)
	{
		mMetaSaveGame->mSavedSlotList.Remove(*Data);
		SaveMetaSaveGame();
	}
}

FDateTime USaveSubsystem::GetSaveFileTimestamp(const FString& SlotName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// 슬롯 이름을 파일 전체 경로로 변환
	FString SaveFilePath = FPaths::ProjectSavedDir() / TEXT("SaveGames") / SlotName + TEXT(".sav");

	if (PlatformFile.FileExists(*SaveFilePath))
	{
		// 파일의 마지막 수정 시간을 가져옵니다.
		return PlatformFile.GetTimeStamp(*SaveFilePath);
	}

	return FDateTime::MinValue();
}

void USaveSubsystem::GetSaveFileData(TMap<FString, FString>& SaveGameData)
{
	for (const auto& SaveData : mMetaSaveGame->mSavedSlotList)
	{
		// 로컬 시간 변환
		//FTimespan LocalOffset = FDateTime::Now() - FDateTime::UtcNow();
		//FDateTime LocalTime = SaveData.SaveDate + LocalOffset;
		SaveGameData.Add(SaveData.SlotName, SaveData.SaveDate.ToString());
	}
}

const FSoundSaveData USaveSubsystem::GetSavedSoundData()
{
	// TODO: 여기에 return 문을 삽입합니다.
	if (!mSoundSaveGame)
	{
		LoadSound();
	}

	return mSoundSaveGame->mSoundSaveData;
}

AActor* USaveSubsystem::GetCurrentPlayer() const
{
	if (UWorld* World = GetWorld())
	{
		if (World->IsGameWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				return PC->GetPawn();
			}
		}
	}
	return nullptr;
}

FString USaveSubsystem::MakeSlotName() const
{
	FGuid NewGuid = FGuid::NewGuid();
	FString SlotName = FString::Printf(TEXT("%s_%s"), *SaveSlotName, *NewGuid.ToString(EGuidFormats::Digits));
	return SlotName;
}


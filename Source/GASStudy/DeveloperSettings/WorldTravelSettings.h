// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "WorldTravelSettings.generated.h"

/**
 * 
 */

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "World Travel Settings", ToolTip = "이동 월드 목록 저장"))
class GASSTUDY_API UWorldTravelSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const UWorldTravelSettings* Get()
	{
		return GetDefault<UWorldTravelSettings>();
	}

public:
	/*TSoftObjectPtr<UWorld> GetTravelLevel(const FName& LevelName) const
	{
		if (!mTravelableLevels.Contains(LevelName))
		{
			return nullptr;
		}

		return mTravelableLevels[LevelName].Get();
	}*/

	FSoftObjectPath GetTravelLevel_V2(const FName& LevelName) const
	{
		if (!mTravelableLevels_V2.Contains(LevelName))
		{
			return nullptr;
		}

		return mTravelableLevels_V2[LevelName];
	}

	TSoftObjectPtr<UWorld> GetLoadingLevel() const
	{
		return LoadingLevel;
	}

private:
	UPROPERTY(EditAnywhere, Config, meta =(DisplayName = "TravelableLevels"))
	TMap<FName, TSoftObjectPtr<UWorld>> mTravelableLevels;

	UPROPERTY(EditAnywhere, Config, meta = (DisplayName = "TravelableLevels_V2"))
	TMap<FName, FSoftObjectPath> mTravelableLevels_V2;

	UPROPERTY(EditAnywhere, Config)
	TSoftObjectPtr<UWorld> LoadingLevel;
};

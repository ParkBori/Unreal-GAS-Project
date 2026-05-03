// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoadingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ALoadingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALoadingGameMode();

protected:
	virtual void BeginPlay() override;


	// InitGameMode 가 BeginPlay 보다 먼저 호출됨 
	// IniGame -> InitGameStae -> Login -> PostLogin -> LogOut
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;


protected:
	FString mMapName;
	FString mMapPath;
	FString bLoadSaveData;
};

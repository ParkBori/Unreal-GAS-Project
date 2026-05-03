// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

public:
	virtual void RestartPlayer(AController* NewPlayer) override;

public:
	//virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	//virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;

private:
	UFUNCTION()
	void Callback_OnGameCleared();

protected:
	UPROPERTY(EditAnywhere, Category = "UI|Credit", meta = (DisplayName = "CreditWidgetClass", Tooltip = "크레디 위젯 클래스"))
	TSubclassOf<class UCreditWidget> mCreditWidgetClass;

private:
	FString bLoadSaveData;

	FTimerHandle mWaitPlayerReadyToEnd;
};

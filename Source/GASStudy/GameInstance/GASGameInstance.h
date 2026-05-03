// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GASGameInstance.generated.h"

/**
 * 프로젝트 전용 게임 인스턴스
 */
UCLASS()
class GASSTUDY_API UGASGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	// 세이브 (동기)
	void SaveGame();

private:
	void Callback_GameExit();
	
#if WITH_EDITOR
	void Callback_EditerEnd(const bool bValue);
#endif

};

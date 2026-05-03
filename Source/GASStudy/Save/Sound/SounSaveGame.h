// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/Sound/Data/SoundSaveData.h"
#include "SounSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API USounSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()	
	FSoundSaveData mSoundSaveData;
};

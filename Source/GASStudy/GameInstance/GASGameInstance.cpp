// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/GASGameInstance.h"

#include "Subsystem/SaveSubsystem.h"

//#if WITH_EDITOR
//#include "Editor.h"
//#endif

void UGASGameInstance::Init()
{
	Super::Init();

	// 모드에 따라서 다르게 작동시킴
#if WITH_EDITOR
	//FEditorDelegates::EndPIE.AddUObject(this, &UGASGameInstance::Callback_EditerEnd);
#endif
	FCoreDelegates::OnPreExit.AddUObject(this, &UGASGameInstance::Callback_GameExit);
}

void UGASGameInstance::SaveGame()
{
	// 동기저장
	auto* SS = GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}

	SS->SaveGameSync();
}

void UGASGameInstance::Callback_GameExit()
{
	SaveGame();
}


#if WITH_EDITOR
void UGASGameInstance::Callback_EditerEnd(const bool bValue)
{
	SaveGame();
}
#endif

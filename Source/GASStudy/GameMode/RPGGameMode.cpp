// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RPGGameMode.h"
#include "Shared/Macro/Log.h"
#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/QuestSubsystem.h"
#include "UI/Credit/CreditWidget.h"
#include "PlayerController/InGamePlayerController.h"

void ARPGGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UQuestSubsystem* QS = GetGameInstance()->GetSubsystem<UQuestSubsystem>())
    {
        QS->mOnGameCleared.RemoveAll(this);
        QS->mOnGameCleared.AddDynamic(this, &ARPGGameMode::Callback_OnGameCleared);
    }
}

void ARPGGameMode::RestartPlayer(AController* NewPlayer)
{
    // 기존 데이터를 사용시 
    // if (bLoadSaveData == TEXT("Load"))
    // {
    //     USaveSubsystem* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
    //     const FTransform& LastSavePoint = SS->GetLastSavePoint();
    //     RestartPlayerAtTransform(NewPlayer, LastSavePoint);
    // }
    // // 새게임시
    // else
    // {
    // }
    Super::RestartPlayer(NewPlayer);
}

void ARPGGameMode::Callback_OnGameCleared()
{
    if (!mCreditWidgetClass)
    {
        return;
    }

    AInGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AInGamePlayerController>();
    if (!IsValid(PC))
    {
        return;
    }

    // 플레이어 종료 준비가 되어있지 않다면 타이머로 대기
    if (!PC->GetPlayerReadyToEnd())
    {
        auto* World = GetWorld();
        if (!IsValid(World))
        {
            return;
        }

        World->GetTimerManager().SetTimer(
            mWaitPlayerReadyToEnd,
            [this]()
            {
                this->Callback_OnGameCleared();
            },
            .5f,
            false
        );

        return;
    }

    // 크레딧 위젯 생성 
    // 종료 및 레벨 처리는 크레딧쪽에서 진행됨
    UCreditWidget* CreditWidgetInstance = CreateWidget<UCreditWidget>(PC, mCreditWidgetClass);
    if (CreditWidgetInstance)
    {
        CreditWidgetInstance->AddToViewport();
        PC->SetInputMode(FInputModeUIOnly());
        PC->bShowMouseCursor = true;
    }
}

APlayerController* ARPGGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* RetPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	FParse::Value(*OptionsString, TEXT("LoadSaveData="), bLoadSaveData);
	return RetPlayerController;
}


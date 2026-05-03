// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/WorldTransitionSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "DeveloperSettings/WorldTravelSettings.h"




//void UWorldTransitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	FWorldDelegates::OnPostLoadMapWithWorld.AddUObject(
//		this, &UWorldTravelSubsystem::HandlePostLoadMap);
//}

void UWorldTransitionSubsystem::RequestTransition(const FTransitionRequest& Request)
{
	mTransitionRequest = Request;
	
	bool bLoadSaveGame = !mTransitionRequest.IsNewGame;

	auto LoadingLevel2 = UWorldTravelSettings::Get()->GetTravelLevel_V2(Request.LevelName);
	//FString Option = FString::Printf(TEXT("MapName=FireStage MapPath=/Game/ASIAN_FireStage/Map/FireStage "));
	FString MapPath = *LoadingLevel2.ToString();
	FString Option = FString::Printf(TEXT("MapPath=%s LoadSaveData=%s "), *MapPath, bLoadSaveGame ? TEXT("Load") : TEXT("NotLoad"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LVL_Loading"), true, Option);
}

//void UWorldTransitionSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
//{
//    if (!LoadedWorld) return;
//
//    if (mTransitionRequest.WorldTransitionType == EWorldTransitionType::Teleport)
//    {
//        if (APlayerController* PC =
//            LoadedWorld->GetFirstPlayerController())
//        {
//            if (APawn* Pawn = PC->GetPawn())
//            {
//                Pawn->SetActorLocation(mTransitionRequest.SpawnLocation);
//            }
//        }
//    }
//
//    // SaveGame 로드
//    if (!mTransitionRequest.SaveSlot.IsNone())
//    {
//        if (USaveGame* SaveObj =
//            UGameplayStatics::LoadGameFromSlot(
//                mTransitionRequest.SaveSlot.ToString(), 0))
//        {
//            // Apply Save Data
//        }
//    }
//}
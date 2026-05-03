// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LoadingGameMode.h"
#include "PlayerController/LoadingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Subsystem/SaveSubsystem.h"

ALoadingGameMode::ALoadingGameMode()
{
	PlayerControllerClass = ALoadingPlayerController::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

void ALoadingGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Current GM: %s"), *GetClass()->GetName());

	if (auto* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		SS->LoadGame(FOnLoadComplete());
	}

	// 비동기로 다음 레벨 로딩
	// FLoadPackageAsyncOptionalParams
	// 로딩이 다되면, 바인딩한 함수가 호출됨.
	// LoadPackageAsync(MapPath, FLoadPackageAsyncDelegate::CreateUObject(this, &ALoadingGameMode::LoadingAsyncComplete), 0, PKG_ContainsMap);
	TWeakObjectPtr<ALoadingGameMode> WeakThis(this);
	auto Handle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		mMapPath,
		[WeakThis]() {
			if (ALoadingGameMode* StrongThis = WeakThis.Get())
			{
				FTimerHandle TimerHandle;
				StrongThis->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [StrongThis]() {
					FString Option = FString::Printf(TEXT("LoadSaveData=%s "), *StrongThis->bLoadSaveData);
					UGameplayStatics::OpenLevel(StrongThis->GetWorld(), *StrongThis->mMapName, true, Option);
					},
					5.f, false);
			}
		},
		FStreamableManager::AsyncLoadHighPriority);
}

APlayerController* ALoadingGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* RetPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	FParse::Value(*OptionsString, TEXT("MapPath="), mMapPath);

	FParse::Value(*OptionsString, TEXT("LoadSaveData="), bLoadSaveData);

	FString Left, Right;
	// "."를 찾아 그 오른쪽(Right)
	if (mMapPath.Split(TEXT("."), &Left, &Right))
	{
		// Right 결과: "A"
		mMapName = Right;
	}

	return RetPlayerController;
}
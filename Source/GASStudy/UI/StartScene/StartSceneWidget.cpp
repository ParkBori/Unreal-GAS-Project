// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartScene/StartSceneWidget.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include "Kismet/GameplayStatics.h"

UStartSceneWidget::UStartSceneWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mWidgetName = TEXT("");

	//static ConstructorHelpers::FObjectFinder<UMediaPlayer> MediaPlayerAsset(TEXT("/Script/MediaAssets.MediaPlayer'/Game/Movies/MP_Start.MP_Start'"));
	//if (MediaPlayerAsset.Succeeded())
	//{
	//	mMediaPlayer = MediaPlayerAsset.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UFileMediaSource> MediaSourceAsset(TEXT("/Script/MediaAssets.FileMediaSource'/Game/Movies/FireStartScene_V3.FireStartScene_V3'"));
	//if (MediaSourceAsset.Succeeded())
	//{
	//	mMediaSource = MediaSourceAsset.Object;
	//}
}

void UStartSceneWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(mMediaPlayer) && IsValid(mMediaSource))
	{
		mMediaPlayer->OpenSource(mMediaSource);
		mMediaPlayer->SetLooping(true);
	}

	mStartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));

	mStartButton->OnHovered.AddDynamic(this, &UStartSceneWidget::OnHovered_StartButton);
	mStartButton->OnUnhovered.AddDynamic(this, &UStartSceneWidget::OnUnHovered_StartButton);
	mStartButton->OnClicked.AddDynamic(this, &UStartSceneWidget::OnClicked_StartButton);
}

void UStartSceneWidget::OnHovered_StartButton()
{
	PlayWidgetAnimation("HoveredStartButton");
}

void UStartSceneWidget::OnUnHovered_StartButton()
{
	PlayWidgetAnimation("HoveredStartButton", 0.f, 1.f, false);
}

void UStartSceneWidget::OnClicked_StartButton()
{
	// 별로 안이쁨 
	// PlayWidgetAnimation("ClickedStartButton");


	// 동기 

	// 비동기 

	// 로딩 -> 게임씬 
	// 우선 로딩화면으로 전환을 시키는데,
	// Option 을 함께 넣어서 전달. 
	// Option 에는 현재는 맵이름만 전달.
	// 마지막에 공백을 넣어줘야 이름을 파싱했을때 정상적으로 나옴 
	FString Option = FString::Printf(TEXT("MapName=FireStage MapPath=/Game/ASIAN_FireStage/Map/FireStage "));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LoadingFireStage"), true, Option);
}

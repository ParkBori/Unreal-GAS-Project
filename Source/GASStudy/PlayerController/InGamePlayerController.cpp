// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/InGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUD/PlayerHUD.h"
#include "GameState/MyGameStateBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Component/InteractComponent.h"

#include "Character/Playable/PlayableCharacter.h"

#include "UI/Pannel/InGameMenuPanel/InGameMenuWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/Dialogue/DialogueHUDWidget.h"
#include "UI/Shop/ShopWidget.h"


#include "Shared/Macro/Log.h"

#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/ItemSubsystem.h"
#include "Subsystem/InventoryPreviewSubsystem.h"
#include "Subsystem/DialogueSubsystem.h"
#include "Subsystem/QuestSubsystem.h"
#include "Subsystem/WorldTransitionSubsystem.h"

#include "GameMode/RPGGameMode.h"


AInGamePlayerController::AInGamePlayerController()
{
	static  ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_UIOnly.IMC_UIOnly'"));
	{
		mUIOnlyIMC = IMCAsset.Object;
	}

	static  ConstructorHelpers::FObjectFinder<UInputAction> OpenGameMenuAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ToggleMenu.IA_ToggleMenu'"));
	if (OpenGameMenuAsset.Succeeded())
	{
		mGameMenuToggleAction = OpenGameMenuAsset.Object;
	}

	static  ConstructorHelpers::FObjectFinder<UInputAction> mRestartActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Restart.IA_Restart'"));
	if (mRestartActionAsset.Succeeded())
	{
		mRestartAction = mRestartActionAsset.Object;
	}
}

void AInGamePlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AInGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// 위젯 생성
	CreateWidgets();

	// 서브시스템 초기화
	InitSubsystem(aPawn);

	// 콘솔 명령어 처리 
	// ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);

	// 플레이어가 죽었을 컨트롤러에서 실행되어 하는 로직 
	if (ARPGCharacter* RPGC = Cast<ARPGCharacter>(aPawn))
	{
		RPGC->mOnDead.RemoveAll(this);
		RPGC->mOnDead.AddUObject(this, &ThisClass::Callback_OnPlayerDeath);
	}
}


void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ChangeInputModeGameOnly();
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (IsValid(Input))
	{
		Input->BindAction(mGameMenuToggleAction,	ETriggerEvent::Started, this, &AInGamePlayerController::ToggleGameMenu);
		Input->BindAction(mRestartAction,			ETriggerEvent::Started, this, &AInGamePlayerController::RestartCurrentLevel);
	}
}

void AInGamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyWidget();
	Super::EndPlay(EndPlayReason); // 부모 함수 호출 필수
}

void AInGamePlayerController::RunDialogue()
{
	mPlayerHUDInstance->SetVisibility(ESlateVisibility::Collapsed);
}



void AInGamePlayerController::SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	Super::SetViewTargetWithBlend(NewViewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);

	auto* OwningCharacter = GetCharacter();
	if (IsValid(NewViewTarget) && IsValid(OwningCharacter))
	{
		if (OwningCharacter == NewViewTarget)
		{
			OwningCharacter->SetActorHiddenInGame(false);
		}
		else
		{
			OwningCharacter->SetActorHiddenInGame(true);
		}
	}
}


void AInGamePlayerController::ToggleGameMenu()
{
	check(mGameMenuClass);
	if (IsValid(mGameMenuClass))
	{
		if (false == mGameMenuOpned)
		{
			auto* IPS = GetWorld()->GetSubsystem<UInventoryPreviewSubsystem>();

			AActor* PreviewActor = IPS->GetPreviewCharacter();
			if (PreviewActor)
			{
				if (IsValid(mPlayerHUDInstance))
				{
					mPlayerHUDInstance->VisibiltyActivate(false);
				}

				SetViewTargetWithBlend(PreviewActor);
			}

			OpenGameMenu();
		}
		else
		{
			if (IsValid(mPlayerHUDInstance))
			{
				mPlayerHUDInstance->VisibiltyActivate(true);
			}

			SetViewTargetWithBlend(GetPawn());
			CloseGameMenu();
		}

		mGameMenuOpned = !mGameMenuOpned;
	}
}

void AInGamePlayerController::OpenGameMenu()
{
	// 게임 메뉴 열기 
	mGameMenuInstance->SetVisibility(ESlateVisibility::Visible);
	
	// 마우스 커서 보이게 하기 + 입력모드 변경 
	ChangeInputModeUIOnly();

	// 게임 일시정지 
	//SetPause(true);
}

void AInGamePlayerController::CloseGameMenu()
{
	// 게임 메뉴 제거 
	mGameMenuInstance->SetVisibility(ESlateVisibility::Collapsed);

	// 마우스 커서 숨기기 + 입력모드 변경 
	ChangeInputModeGameOnly();

	// 게임 일시정지 해제
	//SetPause(false);
}

void AInGamePlayerController::Callback_StartDialogue(AActor* Speaker)
{
	if (!Speaker)
	{
		return;
	}

	if (IsValid(mPlayerHUDInstance))
	{
		mPlayerHUDInstance->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(mDialogueWidgetInstance))
	{
		mDialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		ChangeInputModeUIOnly();

	}

	bIsDialogueEnded = false;	
	SetViewTargetWithBlend(Speaker, .5f);

	IncreaseNeedMouseCursor();


	auto IC = GetPawn()->FindComponentByClass<UInteractComponent>();
	if (IsValid(IC))
	{
		IC->ToggleInteractWidget(false);
	}
}


void AInGamePlayerController::Callback_EndDialogue()
{
	if (IsValid(mPlayerHUDInstance))
	{
		mPlayerHUDInstance->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(mDialogueWidgetInstance))
	{
		mDialogueWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		ChangeInputModeGameOnly();
	}

	bIsDialogueEnded = true;
	SetViewTargetWithBlend(GetPawn(), .5f);

	DecreaseNeedMouseCursor();

	auto IC = GetPawn()->FindComponentByClass<UInteractComponent>();
	if (IsValid(IC))
	{
		IC->ToggleInteractWidget(true);
	}
}


void AInGamePlayerController::Callabck_AddNewQuest(const FQuest* QuestInfo)
{
	// 퀘스트 UI 업데이트 

	if (IsValid(mGameMenuInstance))
	{
		//mGameMenuInstance->Update(QuestInfo, )
	}
}

void AInGamePlayerController::ChangeInputModeUIOnly()
{
	bShowMouseCursor = true;
	FInputModeUIOnly UIOnly;
	SetInputMode(UIOnly);
}

void AInGamePlayerController::ChangeInputModeGameOnly()
{
	bShowMouseCursor = false;
	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
}

void AInGamePlayerController::IncreaseNeedMouseCursor()
{
	mNeedMouseCursor++;
	
	if (mNeedMouseCursor > 0)
	{
		SetShowMouseCursor(true);
	}
}

void AInGamePlayerController::DecreaseNeedMouseCursor()
{
	mNeedMouseCursor--;

	if (mNeedMouseCursor <= 0)
	{
		mNeedMouseCursor = 0;
		SetShowMouseCursor(false);
	}
}

void AInGamePlayerController::CreateWidgets()
{
	// 생성시, 각 위젯 인스턴스가 존재하지 않을 경우에만 생성한다.

	// HUD 생성
	if (mPlayerHUDClass)
	{
		if (mPlayerHUDInstance)
		{
			mPlayerHUDInstance->Init();
		}
		else
		{
			// HUD 생성 
			mPlayerHUDInstance = CreateWidget<UPlayerHUD>(this, mPlayerHUDClass);
			if (IsValid(mPlayerHUDInstance))
			{
				mPlayerHUDInstance->Init();
				mPlayerHUDInstance->AddToViewport();
				mPlayerHUDInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	// 게임메뉴
	if (mGameMenuClass)
	{
		if (mGameMenuInstance)
		{
			mGameMenuInstance->Init();
		}
		else
		{
			mGameMenuInstance = CreateWidget<UInGameMenuWidget>(this, mGameMenuClass);
			if (IsValid(mGameMenuInstance))
			{
				// Init -> AddToViewport 순서 주의
				mGameMenuInstance->AddToViewport();
				mGameMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	// 다이얼로그 생성
	if (mDialogueWidgetClass && !mDialogueWidgetInstance)
	{
		mDialogueWidgetInstance = CreateWidget<UDialogueHUDWidget>(this, mDialogueWidgetClass);
		if (IsValid(mDialogueWidgetInstance))
		{
			mDialogueWidgetInstance->AddToViewport();
			mDialogueWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AInGamePlayerController::CreateShopWidget(FName NPCName, AActor* NewViewTarget)
{
	if (mShopWidgetClass && IsValid(NewViewTarget))
	{
		mShopWidgetInstance = CreateWidget<UShopWidget>(this, mShopWidgetClass);
		if (IsValid(mShopWidgetInstance))
		{
			// Init -> AddToViewport 순서 주의
			mShopWidgetInstance->Init(NPCName);
			mShopWidgetInstance->AddToViewport();

		SetViewTargetWithBlend(NewViewTarget, .5f);
		IncreaseNeedMouseCursor();
		mShopWidgetInstance->OnNativeDestruct.AddWeakLambda(this, [this](UUserWidget* ClosedWidget) {
			DecreaseNeedMouseCursor();
			SetViewTargetWithBlend(GetPawn(), .5f);
			ChangeInputModeGameOnly();
			mPlayerHUDInstance->SetVisibility(ESlateVisibility::Visible	);
			});
		

		mPlayerHUDInstance->SetVisibility(ESlateVisibility::Collapsed);
		ChangeInputModeUIOnly();
		}
	}
}

void AInGamePlayerController::DestroyWidget()
{
	//if (IsValid(mPlayerHUDInstance))
	//{
	//	mPlayerHUDInstance->RemoveFromParent();
	//	mPlayerHUDInstance = nullptr;
	//}
	//
	//if (IsValid(mDialogueWidgetInstance))
	//{
	//	mDialogueWidgetInstance->RemoveFromParent();
	//	mDialogueWidgetInstance = nullptr;
	//}
}

void AInGamePlayerController::InitSubsystem(APawn* aPawn)
{
	UDialogueSubsystem* DialogueSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
	if (IsValid(DialogueSubsystem))
	{
		// 플레이어 리스폰시 OnPossess 함수가 호출되므로 중복 바인딩 가능성이 있으므로, 
		// 제거후 바인딩을 한다.
		DialogueSubsystem->mOnStartDialogueSystem.RemoveAll(this);
		DialogueSubsystem->mOnEndDialogueSystem.RemoveAll(this);

		DialogueSubsystem->mOnStartDialogueSystem.AddUObject(this, &AInGamePlayerController::Callback_StartDialogue);
		DialogueSubsystem->mOnEndDialogueSystem.AddUObject(this, &AInGamePlayerController::Callback_EndDialogue);
	}

	// 필요할때 얻어오는 방식을 사용해야함
	// 죽었을때 플레이어를 디스트로이 시키면, 여기서 주입되 정보들은 결국 무용지물이 되기 때문이다.
	UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->Init(aPawn);
	}

	USaveSubsystem* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (SS)
	{
		SS->SetDataByLoadedSaveGame();
	}
}

void AInGamePlayerController::Callback_OnPlayerDeath()
{
	MY_LOG(Error, "컨트롤러 플레이어 정리 및 리스폰 로직 실행");

	if (!mSpawnPlayerCharacter)
	{
		MY_LOG(Error, "스폰 캐릭터 미지정");
		return;
	}

	// 1. 입력 차단
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	// 2. 사망 UI 띄우기 (블루프린트에서 구현된 이벤트를 호출하거나 위젯 생성)
	// ShowDeathScreen();

	// 3. 일정 시간 후 리스폰 시작
	FTimerHandle RespawnTimer;
	// GetWorldTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RestartLevelAtSavePoint, 3.0f, false);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RestartCurrentLevel, 3.0f, false);
}

void AInGamePlayerController::RestartLevelAtSavePoint()
{
	// 4. 화면 페이드 아웃 (로딩 연출)
	PlayerCameraManager->StartCameraFade(0.f, 1.f, 1.0f, FLinearColor::Black, false, true);

	// 5. 실제 리스폰 (약간의 지연 후 실행)
	FTimerHandle LoadTimer;
	GetWorldTimerManager().SetTimer(LoadTimer, [this]()
		{
			// 현재 폰 파괴
			if (GetPawn()) GetPawn()->Destroy();

			if (auto* GM = GetWorld()->GetAuthGameMode<ARPGGameMode>())
			{
				GM->RestartPlayer(this);
			}

			// 8. 화면 페이드 인
			PlayerCameraManager->StartCameraFade(1.f, 0.f, 1.0f, FLinearColor::Black, false, false);
			SetIgnoreMoveInput(false);
			SetIgnoreLookInput(false);

		}, 1.2f, false);
}

void AInGamePlayerController::RestartCurrentLevel()
{
	auto* TS = GetGameInstance()->GetSubsystem<UWorldTransitionSubsystem>();
	if (!TS)
	{
		return;
	}

	FTransitionRequest TR;
	if (TS->GetPendingRequest().LevelName.IsNone())
	{
		FString CleanLevelName = GetWorld()->GetMapName();
		CleanLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (CleanLevelName == TEXT("Showcase"))
		{
			TR.LevelName = TEXT("Boss");
		}
		else if (CleanLevelName == TEXT("Lvl_Temple"))
		{
			TR.LevelName = TEXT("Lvl_Temple");
		}
		else
		{
			TR.LevelName = TEXT("Main");
		}

		TR.IsNewGame = false;
	}
	else
	{
		TR = TS->GetPendingRequest();
	}
	TS->RequestTransition(TR);
}

void AInGamePlayerController::StartFade(float Duration, FOnFadeCompleted OnFadeCompleted)
{
	auto PCM = PlayerCameraManager;
	if (!IsValid(PCM))
	{
		return;
	}

	// HUD 위젯 제거 
	mDialogueWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
	mGameMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	mPlayerHUDInstance->SetVisibility(ESlateVisibility::Collapsed);

	PCM->StartCameraFade(0.f, 1.f, Duration, FLinearColor::Black);
	GetWorld()->GetTimerManager().SetTimer(
		mFadeTimerHandle,
		[OnFadeCompleted]()
		{
			OnFadeCompleted.ExecuteIfBound();
		},
		Duration,
		false
	);
}


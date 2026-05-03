// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"


DECLARE_DYNAMIC_DELEGATE(FOnFadeCompleted);

class UPlayerHUD;
class UInGameMenuWidget;
class UDialogueHUDWidget;
class UShopWidget;




/**
 * 
 */
UCLASS()
class GASSTUDY_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AInGamePlayerController();
	//virtual void OnPossess(APawn* aPawn) override;


protected:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void RunDialogue();

	void SetBoss(AActor* Boss);

	UFUNCTION(BlueprintCallable)
	void StartFade(float Duration, FOnFadeCompleted FadeCompleted);

	bool GetPlayerReadyToEnd() const
	{
		return bIsDialogueEnded; // bisReadyToEnd;
	}

	virtual void SetViewTargetWithBlend(class AActor* NewViewTarget, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false) override;


protected:
	UFUNCTION(BlueprintCallable)
	void ToggleGameMenu();

	void OpenGameMenu();
	void CloseGameMenu();

	void Callback_StartDialogue(AActor* Speaker);
	void Callback_EndDialogue();
	void Callabck_AddNewQuest(const struct FQuest* QuestInfo);
	
	void ChangeInputModeUIOnly();
	void ChangeInputModeGameOnly();

	void IncreaseNeedMouseCursor();
	void DecreaseNeedMouseCursor();



private:
	// ==== 위젯 생성 ====
	void CreateWidgets();

	// ==== 위젯 파괴 ====
	void DestroyWidget();

	// ==== 서브시스템 초기화 ==== 
	void InitSubsystem(APawn* aPawn);

	// ==== 플레이어 죽음 - 리스폰 ==== 
	void Callback_OnPlayerDeath();
	void RestartLevelAtSavePoint();
	void RestartCurrentLevel();

	// 위젯관리 

public:
	UFUNCTION(BlueprintCallable)
	void CreateShopWidget(FName NPCName, AActor* NewViewTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI|HUD")
	TSubclassOf<UPlayerHUD> mPlayerHUDClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI|HUD")
	TObjectPtr<UPlayerHUD> mPlayerHUDInstance;

	UPROPERTY(EditAnywhere, Category = "UI|Dialogue")
	TSubclassOf<UDialogueHUDWidget> mDialogueWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Dialogue")
	TObjectPtr<UDialogueHUDWidget> mDialogueWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI|Shop")
	TSubclassOf<UShopWidget> mShopWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Shop")
	TObjectPtr<UShopWidget> mShopWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> mUIOnlyIMC;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Menu")
	TObjectPtr<class UInputAction> mGameMenuToggleAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Restart")
	TObjectPtr<class UInputAction> mRestartAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Menu")
	TObjectPtr<class UInputAction> mWheelPannelToggleAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Menu")
	TSubclassOf<UInGameMenuWidget> mGameMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Menu")
	TObjectPtr<UInGameMenuWidget> mGameMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnCharacter", meta = (Tooltip = "컨트롤러가 스폰시킬 캐릭터 클래스", DisplayName = "SpawnCharacter"))
	TSubclassOf<class APlayableCharacter> mSpawnPlayerCharacter;

	bool mGameMenuOpned = false;

	FTimerHandle mFadeTimerHandle;

	bool bIsDialogueEnded = false;

	int32 mNeedMouseCursor = 0;
};



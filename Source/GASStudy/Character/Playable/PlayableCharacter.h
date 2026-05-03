// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/RPGCharacter.h"
#include "Interface/CombatInterface.h"
#include "Interface/GameplayCameraInterface.h"
#include "Shared/Commbat/CombatData.h"
#include "PlayableCharacter.generated.h"

/**
 * 플레이어 키입력 GAS 아이디설정
 * 
 * 현재 어빌리티 내부에서 수동적으로 
 * 입력처리를 진행하고 있어서 
 * 약간의 하드코딩이 진행되는데..
 */
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None			= 0,
	NoramlAttack	= 1,
	Block			= 2,
	Dodge			= 3,
	ToggleEquip		= 4,
	Jump			= 5,
	UseItem			= 6,
};




/**
 *
 */
UCLASS()
class GASSTUDY_API APlayableCharacter 
	: public ARPGCharacter
	// , public ICombatInterface
	//, public IGameplayCameraInterface
{
	GENERATED_BODY()
	
public:
	APlayableCharacter();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:
	void CreateComponent();
	void UpdateCamera(bool Interp);
	void SetCameraEffect();

	const FPlayerPropertiesForCamera GetCameraProperties();

	void MoveAction(const FInputActionValue& Value);
	void LoockAction(const FInputActionValue& Value);
	void RightMouseClickAction();
	void RightMouseReleaseAction();
	void TargetLockAction(const FInputActionValue& Value);
	void InteractAction();
	void UseItemAction();


	/**
	* GAS 발동함수
	*/
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	void InputCombatAction(int32 InputID);

	void InputCombatCancleActivate();
	void InputCombatCancleUnActivate();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void JumpAction(const FInputActionValue& Value);

	void TargetLockActionLeft();

	void TargetLockActionRight();

	void ZoomIn();

	void ZoomOut();


	// 아래 함수들은 하위 개체에서 
	// 추가로직이 필요한 경우
	// 오버라이드를 위해 
	// virtual 이 아닌 UFUNCTION(BlueprintNative) 
	// 로 작성하고, cpp Implelements 를 붙여줘야한다.
	virtual void MyMove(const FVector2D& Axis);
	virtual void MyLook(const FVector2D& Axis);

	virtual void BlockOrHeavyAttack();
	virtual void BlockRelease();
	void Dash_V2(const FInputActionValue& Value);
	void Resurrection();


protected:
	void SetCanComboAttackInput(bool Value) { mCanComboAttackInput = Value; }

	UFUNCTION()
	void SetTimeDilation(float NewDilationTime);


public:
	class UEquipComponent* GetEquipComponent();
	class UQuestComponent* GetQuestComponent();
	class UInventoryComponent_V2* GetInventoryComponent();


	virtual void SetFinisher(bool NewValue);

	void Slowmotion();
	
	UFUNCTION(BlueprintCallable)
	void PostProcessBlackAndWhileEffect();


private:
	virtual void BindDelegateToAbilitySystem() override;

	virtual void EventDeath(const FGameplayTag Tag, int32 NewCount) override;
	void EventUpdateAnimByEquipWeapon(const FGameplayTag Tag, int32 NewCount);
	void EventUpdateAnimByBlockState(const FGameplayTag Tag, int32 NewCount);

public:
	//virtual FVector GetLockPoint() override;
	void StopTargetLock(bool Value) ;
	AActor* GetTarget() const;
	void SetStrafe(bool Value);
	void SetAim(bool Value);

	// GameplayCameraInterface override 
	// virtual FPlayerPropertiesForCamera Get_CharacterPropertiesForCamera() override;
	// virtual bool Get_PlayerWantToFoucsToTarget() override;
	// virtual void UpdateTargerLocation() override;

	void SwitchCamer(ECameraType CameraType);

	UFUNCTION(BlueprintCallable)
	void TestApplyEffect(TSubclassOf<UGameplayEffect> TestGE);

	// 콘솔 테스트 함수
	UFUNCTION(Exec)
	void Test_PrintString(const FString& DebugMessage);

	UFUNCTION(Exec)
	void Test_AddGameplayTag(const FName& AddTagName);

	UFUNCTION(Exec)
	void Test_RemoveGameplayTag(const FName& RemoveTagName);

	UFUNCTION(Exec)
	void Test_DirectDead();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Finisher;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Dead;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_TargetLock;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_MouseWheelUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_MouseWheelDown;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ZoomIn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ZoomOut;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Block;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_CombatCancle;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_NormalAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_T;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Resurrection;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_UseItem;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToggleEquip;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;


	//TObjectPtr<UInputAction> mIA_DodgeRoll;

	// Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> DefaultCameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> FinisherFrontCameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> FinisherBackCameraBoom;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> MainCamera;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<class UTargetLockComponent> TargetLockComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<class UInventoryComponent_V2> InventoryComponent_V2;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<class UEquipComponent> EquipComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<class UInteractComponent> InteractComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UQuestComponent> QuestComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UMotionWarpingComponent> mMotionWarpingComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UPlayerCombatComponent> mPlayerCombatComp;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<class UWeaponTraceComponent> mWeaponTraceComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UEquipVisualComponent> mEquipVisualComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCharacterAttributeSet> mCharacterAttributeSet;

	//UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	//TObjectPtr<class UGameplayCameraComponent> GameplayCameraComponent;

	bool mPressRightMouse = false; 

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool mCanComboAttackInput = false;



	UPROPERTY(BlueprintReadOnly)
	ECameraStyle CamStyle = ECameraStyle::Balanced;

	UPROPERTY(BlueprintReadOnly)
	FCharacterInput CharacterInput;


	UPROPERTY(BlueprintReadWrite)
	bool TestValue = false;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UDataTable>	DT_CameraInfo;


	bool TargetLockSwitch = false;

	/*
	* 패리태그가 리무브 되는 순간에 State Blocking 이없다면
	* EndAbility 를 쏴주면 될거같은데
	*/
	FDelegateHandle TagAddHandle;
	FDelegateHandle TagRemoveHandle;

	bool bFinisherSetting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Effect")
	TMap <FName, TObjectPtr<UMaterialInterface>> mCameraEffectList;

	UPROPERTY(VisibleAnyWhere, Category = "Camera|Effect")
	TMap<FName, TObjectPtr<UMaterialInstanceDynamic>> mCameraEffectInstanceList;

	ECameraType mCurrentCameraType = ECameraType::Main;
};



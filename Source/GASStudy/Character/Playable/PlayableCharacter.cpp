// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Playable/PlayableCharacter.h"

// Input
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

// GAS 
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "DeveloperSettings/GameDataSettings.h"

// Util
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Shared
#include "Animation/AnimData/AnimDataModel.h"
#include "Shared/Tag/SharedTag.h"
#include "Shared/Anim/LinkAnimInstanceData.h"
#include "Tag/GameplayTag.h"

// Component
#include "Component/InteractComponent.h"
#include "Components/TimelineComponent.h"
#include "Component/FootIKComponent.h"
#include "Component/TargetLockComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Component/WeaponTraceComponent.h"
#include "Component/QuestComponent.h"
#include "Component/InventoryComponent_V2.h"
#include "Component/EquipComponent.h"
#include "Component/PlayerCombatComponent.h"
#include "Component/EquipVisualComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/BoxComponent.h"


APlayableCharacter::APlayableCharacter()
{
	// 에셋 설정 
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (IMCAsset.Succeeded())
	{
		IMC = IMCAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_FinisherAsset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Finisher.IMC_Finisher'"));
	if (IMC_FinisherAsset.Succeeded())
	{
		IMC_Finisher = IMC_FinisherAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_DeadAsset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Dead.IMC_Dead'"));
	if (IMC_DeadAsset.Succeeded())
	{
		IMC_Dead = IMC_DeadAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (MoveActionAsset.Succeeded())
	{
		IA_Move = MoveActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (LookActionAsset.Succeeded())
	{
		IA_Look = LookActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TargetLockActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_TargetLock.IA_TargetLock'"));
	if (TargetLockActionAsset.Succeeded())
	{
		IA_TargetLock = TargetLockActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MouseWheelUpAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MouseWheelUp.IA_MouseWheelUp'"));
	if (IA_MouseWheelUpAsset.Succeeded())
	{
		IA_MouseWheelUp = IA_MouseWheelUpAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MouseWheelDownAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MouseWheelDown.IA_MouseWheelDown'"));
	if (IA_MouseWheelDownAsset.Succeeded())
	{
		IA_MouseWheelDown = IA_MouseWheelDownAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ZoomInAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ZoomIn.IA_ZoomIn'"));
	if (IA_ZoomInAsset.Succeeded())
	{
		IA_ZoomIn = IA_ZoomInAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ZoomOutAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ZoomOut.IA_ZoomOut'"));
	if (IA_ZoomOutAsset.Succeeded())
	{
		IA_ZoomOut = IA_ZoomOutAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_BlockAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Block.IA_Block'"));
	if (IA_BlockAsset.Succeeded())
	{
		IA_Block = IA_BlockAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CombatCancleAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_CombatCancle.IA_CombatCancle'"));
	if (IA_CombatCancleAsset.Succeeded())
	{
		IA_CombatCancle = IA_CombatCancleAsset.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> IA_NormalAttackAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_NoramlAttack.IA_NoramlAttack'"));
	if (IA_NormalAttackAsset.Succeeded())
	{
		IA_NormalAttack = IA_NormalAttackAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DodgeAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Dodge.IA_Dodge'")); 
	if (IA_DodgeAsset.Succeeded())
	{
		IA_Dodge = IA_DodgeAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InteractAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Interact.IA_Interact'"));
	if (IA_InteractAsset.Succeeded())
	{
		IA_Interact = IA_InteractAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ResurrectionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Resurrection.IA_Resurrection'"));
	if (IA_ResurrectionAsset.Succeeded())
	{
		IA_Resurrection = IA_ResurrectionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_UseItemAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_UseItem.IA_UseItem'"));
	if (IA_UseItemAsset.Succeeded())
	{
		IA_UseItem = IA_UseItemAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ToggleEquipAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ToggleEquip.IA_ToggleEquip'"));
	if (IA_UseItemAsset.Succeeded())
	{
		IA_ToggleEquip = IA_ToggleEquipAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JumpAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (IA_JumpAsset.Succeeded())
	{
		IA_Jump = IA_JumpAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CmeraInfoAsset(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Camera/DT_CameraInfo.DT_CameraInfo'"));
	if (DT_CmeraInfoAsset.Succeeded())
	{
		DT_CameraInfo = DT_CmeraInfoAsset.Object;
	}

	
	mCharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	// 컴포넌트 설정
	CreateComponent();
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("beginPlay"));

	// 지금은 이렇게 했지만.. 아마 GAS 원하는 인풋바인딩이 존재하는걸로 알고있음 
	// 계층구조가 정리 되면 그때 맞춰서 리펙토링 필요함

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (IsValid(EnhancedInputLocalPlayerSubsystem))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC, 0);
		}
	}


	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0);
	FOnTimelineFloat TimeLineFloat;
	TimeLineFloat.BindUFunction(this, TEXT("SetTimeDilation"));
	SlowmotionTimelineComponent->AddInterpFloat(SlowmotionCurve, TimeLineFloat);
	//FOnTimelineEvent TimelineEnd;
	//TimelineEnd.BindUFunction(this, TEXT("EndHitStop"));
	//SlowmotionTimelineComponent->SetTimelineFinishedFunc(TimelineEnd);
	//SlowmotionTimelineComponent->PlayFromStart();

	//TagRemoveHandle = AbilitySystemComponent->RegisterGameplayTagEvent(TAG_STATE_PARRYING, EGameplayTagEventType::NewOrRemoved)
	//	.AddUObject(this, &APlayableCharacter::StateParryingTagRemovedCallback);


	// 월드에 배치된 포스트프로세스 가져오기 
	
	// TODO(251027) 플레이어 카메라로 옮겨줘야힘
	//APostProcessVolume* PostProcess = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
	//if (IsValid(PostProcess))
	//{
	//	// FPostProcessSettings 은 포스트프로세의 모든 세팅값이다
	//	// 에디터에서 확인할 수 있는 모든 값들을 담고있음 
	//	FPostProcessSettings& Settings = PostProcess->Settings;

	//	/*
	//	*이것은 FWeightedBlendables 타입이며, 여러 개의 포스트 프로세스 블렌더블(Blendable) 객체들을 관리하는 컨테이너입니다.
	//	*블렌더블(Blendable)**이라는 것은 다른 포스트 프로세스 설정과 "블렌딩"될 수 있는 특정 포스트 프로세스 효과의 집합을 의미합니다
	//	*/
	//	int32	Count = Settings.WeightedBlendables.Array.Num();

	//	for (int32 i = 0; i < Count; ++i)
	//	{
	//		UMaterialInterface* OriginMtrl = Cast<UMaterialInterface>(Settings.WeightedBlendables.Array[i].Object);
	//		if (IsValid(OriginMtrl))
	//		{
	//			// 기존의 머티리얼을 런타임에 변경시킬 수 있게 다이나믹 상태로 만듦
	//			UMaterialInstanceDynamic* DynamicMtrl = UMaterialInstanceDynamic::Create(OriginMtrl, this);

	//			// 기존 머티리얼을 다이나믹 머티리얼로 교체
	//			Settings.WeightedBlendables.Array[i].Object = DynamicMtrl;

	//			// 해당 머티리얼을 캐싱
	//			mPostProcessMtrls.Add(DynamicMtrl);
	//		}
	//	}
	//}
}

void APlayableCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCamera(true);
}

void APlayableCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayableCharacter::MoveAction);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayableCharacter::LoockAction);

		EnhancedInputComponent->BindAction(IA_TargetLock, ETriggerEvent::Triggered, this, &APlayableCharacter::TargetLockAction);
		//EnhancedInputComponent->BindAction(IA_TargetLock, ETriggerEvent::Completed, this, &APlayableCharacter::TargetLockAction);

		EnhancedInputComponent->BindAction(IA_ZoomIn, ETriggerEvent::Started, this, &APlayableCharacter::ZoomIn);
		EnhancedInputComponent->BindAction(IA_ZoomOut, ETriggerEvent::Started, this, &APlayableCharacter::ZoomOut);

		EnhancedInputComponent->BindAction(IA_MouseWheelUp, ETriggerEvent::Started, this, &APlayableCharacter::TargetLockActionLeft);
		EnhancedInputComponent->BindAction(IA_MouseWheelDown, ETriggerEvent::Started, this, &APlayableCharacter::TargetLockActionRight);
	
	
		
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayableCharacter::InteractAction);

		// GAS Input
		// EnhancedInputComponent->BindAction(IA_SecondaryAttack, ETriggerEvent::Started, this, &APlayableCharacter::GASInputPressed, (int32)EAbilityInputID::SecondaryAttack);
		// EnhancedInputComponent->BindAction(IA_SecondaryAttack, ETriggerEvent::Completed, this, &APlayableCharacter::GASInputReleased, (int32)EAbilityInputID::SecondaryAttack);

		EnhancedInputComponent->BindAction(IA_NormalAttack, ETriggerEvent::Started,		this, &APlayableCharacter::InputCombatAction, (int32)EAbilityInputID::NoramlAttack);
		EnhancedInputComponent->BindAction(IA_Block,		ETriggerEvent::Started,		this, &APlayableCharacter::GASInputPressed, (int32)EAbilityInputID::Block);
		EnhancedInputComponent->BindAction(IA_Block,		ETriggerEvent::Completed,	this, &APlayableCharacter::GASInputReleased, (int32)EAbilityInputID::Block);
		EnhancedInputComponent->BindAction(IA_Dodge,		ETriggerEvent::Started,		this, &APlayableCharacter::InputCombatAction, (int32)EAbilityInputID::Dodge);
		EnhancedInputComponent->BindAction(IA_ToggleEquip,	ETriggerEvent::Started,		this, &APlayableCharacter::InputCombatAction, (int32)EAbilityInputID::ToggleEquip);
		EnhancedInputComponent->BindAction(IA_UseItem,		ETriggerEvent::Started,		this, &APlayableCharacter::InputCombatAction, (int32)EAbilityInputID::UseItem);
		EnhancedInputComponent->BindAction(IA_CombatCancle,	ETriggerEvent::Started,		this, &APlayableCharacter::InputCombatCancleActivate);
		EnhancedInputComponent->BindAction(IA_CombatCancle,	ETriggerEvent::Completed,	this, &APlayableCharacter::InputCombatCancleUnActivate);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayableCharacter::GASInputPressed, (int32)EAbilityInputID::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlayableCharacter::GASInputReleased, (int32)EAbilityInputID::Jump);
	}
}

void APlayableCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(AbilitySystemComponent))
	{
		InventoryComponent_V2->Init(AbilitySystemComponent);
		EquipComponent->Init(AbilitySystemComponent);

	}

	SetCameraEffect();
}

void APlayableCharacter::CreateComponent()
{
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (CMC)
	{
		CMC->bUseControllerDesiredRotation = false;
		CMC->bOrientRotationToMovement = true;
		CMC->RotationRate = FRotator(0.f, 300.f, 0.f);
		CMC->MaxWalkSpeed = 300.f;
	}

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	{
		DefaultCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
		DefaultCameraBoom->SetupAttachment(RootComponent);
		DefaultCameraBoom->TargetArmLength = 240.0f;
		DefaultCameraBoom->SocketOffset = FVector(0.f, 60.f, 20.f);
		DefaultCameraBoom->bUsePawnControlRotation = true;
		DefaultCameraBoom->bEnableCameraLag = true;
		DefaultCameraBoom->CameraLagSpeed = 3.0f;


		MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
		MainCamera->SetupAttachment(DefaultCameraBoom);
	
		FinisherFrontCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FinisherFrontSpringArmComponent"));
		FinisherFrontCameraBoom->SetupAttachment(GetMesh(), TEXT("hand_r"));
		FinisherFrontCameraBoom->TargetArmLength = 200.f;

		FinisherBackCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FinisherBackSpringArmComponent"));
		FinisherBackCameraBoom->SetupAttachment(GetMesh());
		FinisherBackCameraBoom->SetRelativeRotation(FRotator(-20.f, -130.f, 0.f));
		FinisherBackCameraBoom->TargetArmLength = 400.f;
	}

	SetStrafe(false);

	TargetLockComponent = CreateDefaultSubobject<UTargetLockComponent>(TEXT("TargetLock"));
	InventoryComponent_V2 = CreateDefaultSubobject<UInventoryComponent_V2>(TEXT("InventoryComponent_V2"));
	EquipComponent = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	mMotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
	mPlayerCombatComp = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	mWeaponTraceComp = CreateDefaultSubobject<UWeaponTraceComponent>(TEXT("WeaponTraceComponent"));
	mEquipVisualComp = CreateDefaultSubobject<UEquipVisualComponent>(TEXT("EquipVisualComponent"));
	
	// GameplayCamera 세팅 
	//GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	//GameplayCameraComponent->SetupAttachment(GetMesh());
}

void APlayableCharacter::UpdateCamera(bool Interp)
{
	// TODO : Update Camera 
	const FPlayerPropertiesForCamera CameraProperties =	GetCameraProperties();
	FName CameraName;

	// Hard Target Lock -> On
	// 터겟락 시스템에서 타겟을 쫓아야 하므로, 여기서는 아무것도 하지 않음 
	if (TargetLockComponent->IsTargetLocking())
	{
		// 타겟락 컴포넌트에서 카메라 움직임을 조잘할거임. 여기서는 보간 처리 없이 한번만 
		// 타겟락 위치로 카메라를 보간 시켜주면 됨 
		//UE_LOG(LogTemp, Warning, TEXT("HardLock"));
		
		CameraName = TEXT("TargetLock");
	}

	else
	{
		if (EStance::Crouch == CameraProperties.Stance)
		{
			// Crouch 상태의 카메라 정보 
		}



		switch (CameraProperties.CameraStyle)
		{
		case ECameraStyle::Close:
		{
			switch (CameraProperties.CameraMode)
			{
			case ECameraMode::Aim:
				CameraName = TEXT("Close_Aim");
				break;
			case ECameraMode::Strafe:
				CameraName = TEXT("Close_Strafe");
				break;
			case ECameraMode::FreeCam:
				CameraName = TEXT("Close_Free");
				break;
			}
		}
		break;
		// 
		case ECameraStyle::Balanced:
		{
			switch (CameraProperties.CameraMode)
			{
			case ECameraMode::Aim:
				CameraName = TEXT("Balanced_Aim");
				break;
			case ECameraMode::Strafe:
				CameraName = TEXT("Balanced_Strafe");
				break;
			case ECameraMode::FreeCam:
				CameraName = TEXT("Balanced_Free");
				break;
			}
		}
		break;
		// 
		case ECameraStyle::Far:
		{
			switch (CameraProperties.CameraMode)
			{
			case ECameraMode::Aim:
				CameraName = TEXT("Far_Aim");
				break;
			case ECameraMode::Strafe:
				CameraName = TEXT("Far_Strafe");
				break;
			case ECameraMode::FreeCam:
				CameraName = TEXT("Far_Free");
				break;
			}
		}
		break;
		//
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Current Camera : %s"), *CameraName.ToString());
	FCameraInfo* CameraInfo = DT_CameraInfo->FindRow<FCameraInfo>(CameraName, TEXT(""));
	
	check(CameraInfo);
	//if (CameraInfo)
	{
		// TODO : CameraInfo 정보를 바탕으로 카메라 위치 업데이트 
		float Delta = GetWorld()->GetDeltaSeconds();
		float TransitionSpeed = CameraInfo->TransitionSpeed;

		float  CurrentFOV = MainCamera->FieldOfView;
		float TargetFov = CameraInfo->FieldOfView;

		// FOV 보간
		MainCamera->FieldOfView = FMath::FInterpTo<float>(CurrentFOV, TargetFov, Delta, TransitionSpeed);


		float CurrentArmLen = DefaultCameraBoom->TargetArmLength;
		float TargetArmLen = CameraInfo->SpringArmLength;

		// 타겟암 길이 보간 
		DefaultCameraBoom->TargetArmLength = FMath::FInterpTo<float>(CurrentArmLen, TargetArmLen, Delta, TransitionSpeed);


		float CurrentCameraLag = DefaultCameraBoom->CameraLagSpeed;
		float TargetCameraLag = CameraInfo->TranslationLagSpeed;

		// 카메라 렉 보간 
		DefaultCameraBoom->CameraLagSpeed = FMath::FInterpTo<float>(CurrentCameraLag, TargetCameraLag, Delta, TransitionSpeed);


		FVector CurrentSocketOffset = DefaultCameraBoom->SocketOffset;
		FVector TargetSocketOffset = CameraInfo->SocketOffset;

		// 카메라 렉 보간 
		DefaultCameraBoom->SocketOffset = FMath::VInterpTo(CurrentSocketOffset, TargetSocketOffset, Delta, TransitionSpeed);
	}
}

void APlayableCharacter::SetCameraEffect()
{
	for (const auto& pMI : mCameraEffectList)
	{
		UMaterialInstanceDynamic* MID =  UMaterialInstanceDynamic::Create(pMI.Value, this);
		mCameraEffectInstanceList.Add(pMI.Key, MID);
		
		MainCamera->PostProcessSettings.AddBlendable(MID, 1.0f);
	}
}

void APlayableCharacter::MoveAction(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();
	MyMove(Axis);
}

void APlayableCharacter::LoockAction(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();
	MyLook(Axis);
}

void APlayableCharacter::RightMouseClickAction()
{
	UE_LOG(LogTemp, Display, TEXT("APlayableCharacter::RightMouseClickAction Pressed"));
	BlockOrHeavyAttack();
}

void APlayableCharacter::RightMouseReleaseAction()
{
	UE_LOG(LogTemp, Display, TEXT("APlayableCharacter::RightMouseClickAction Released"));
	BlockRelease();
}


void APlayableCharacter::TargetLockActionLeft()
{
	TargetLockComponent->SwitchTargetLeft();
}

void APlayableCharacter::TargetLockActionRight()
{
	TargetLockComponent->SwitchTargetRight();
}

void APlayableCharacter::ZoomIn()
{
	switch (CamStyle)
	{
	case ECameraStyle::Close:
		break;
	case ECameraStyle::Balanced:
		CamStyle = ECameraStyle::Close;
		UE_LOG(LogTemp, Warning, TEXT("CamStyle : Close"));
		break;
	case ECameraStyle::Far:
		CamStyle = ECameraStyle::Balanced;
		UE_LOG(LogTemp, Warning, TEXT("CamStyle : Balanced"));
		break;
	}
}

void APlayableCharacter::ZoomOut()
{
	switch (CamStyle)
	{
	case ECameraStyle::Close:
		CamStyle = ECameraStyle::Balanced;
		UE_LOG(LogTemp, Warning, TEXT("CamStyle : Balanced"));
		break;
	case ECameraStyle::Balanced:
		CamStyle = ECameraStyle::Far;
		UE_LOG(LogTemp, Warning, TEXT("CamStyle : Far"));
		break;
	case ECameraStyle::Far:
		break;
	}
}

void APlayableCharacter::MyMove(const FVector2D& Axis)
{
	// 컨트롤러의 회전 정보 (Yaw만 사용)
	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	// 전방 벡터 (X 방향)
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// 오른쪽 벡터 (Y 방향)
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 이동
	AddMovementInput(ForwardDirection, Axis.Y);
	AddMovementInput(RightDirection, Axis.X);
}

void APlayableCharacter::MyLook(const FVector2D& Axis)
{
	//UE_LOG(LogTemp, Display, TEXT("%s"), *(Axis.ToString()));
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void APlayableCharacter::BlockOrHeavyAttack()
{
	//bool IsAttackNow = IsAttacking();

	//if (true == IsAttackNow)
	//{
	//	// 현재 RightButton 을 눌렀다는걸 인지해야함 
	//	mPressRightMouse = true;
	//	DoNextAttack();
	//	mPressRightMouse = false;
	//}
	//else if (true == IsBlocking())
	//{
	//	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_BLOCKREINPUT, FGameplayEventData());
	//}
	//else
	{
		// Block 
		ActivateAbilityBySlot(EAbilitySlot::Block);
	}
}

void APlayableCharacter::BlockRelease()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_BLOCKEND, FGameplayEventData());
}


void APlayableCharacter::Dash_V2(const FInputActionValue& Value)
{
	bool Pushed =  Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("Test : %s"), *Value.ToString());

	if (Pushed)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_DashStart, FGameplayEventData());
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_DashEnd, FGameplayEventData());
	}
}



void APlayableCharacter::Resurrection()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		bool bHasIMC_Finisher = EnhancedInputLocalPlayerSubsystem->HasMappingContext(IMC_Dead);
		{
			if (IsValid(EnhancedInputLocalPlayerSubsystem))
			{
				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(IMC_Dead);
			}
		}
	}

	AbilitySystemComponent->RemoveLooseGameplayTag(TAG_STATE_DEATH);
	ActivateAbilityWithTags(FGameplayTagContainer(TAG_PLAYER_RESURRECTION));
}




void APlayableCharacter::JumpAction_Implementation(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("MyJump_Implementation"));
}

void APlayableCharacter::SetStrafe(bool Value)
{
	UCharacterMovementComponent* CMC =  GetCharacterMovement();
	if (CMC)
	{
		if (true == Value && (CurrentGait != EGait::Run))
		{
			CMC->bOrientRotationToMovement = false;
			//CMC->bUseControllerDesiredRotation = true;
			CharacterInput.WantsToStrafe = true;
		}
		else
		{
			CMC->bOrientRotationToMovement = true;
			//CMC->bUseControllerDesiredRotation = false;
			CharacterInput.WantsToStrafe = false;
		}
	}
}

void APlayableCharacter::SetAim(bool Value)
{
	CharacterInput.WantsToAim = Value;
}

const FPlayerPropertiesForCamera APlayableCharacter::GetCameraProperties()
{
	// TODO : 
	/*
	├───Chararcter Properties for Camera	(구조체)		
	│		├───E Camera Style 
	│		│		└───Far 
	│		│		└───Balanced	 
	│		│		└───Close       
	│		│			
	│		├───E Camera Mode
	│		│		└───FaceCam				
	│		│		└───Strafe    
	│		│		└───Aim        
	│		│			
	│		├───E Stace
	│		│		└───Stand
	│		│		└───Crouch  
	*/

	FPlayerPropertiesForCamera RetCameraProperties;

	// Close / Balanced / Far
	RetCameraProperties.CameraStyle = CamStyle;
	
	// Crouch / Normal 
	if (EStance::Crouch == CurrentStance)
	{
		RetCameraProperties.Stance = EStance::Crouch;
	}
	else
	{
		RetCameraProperties.Stance = EStance::Normal;
	}

	// FreeCamera / Strafe 
	ECameraMode TempCamMode = ECameraMode::FreeCam;
	if (CharacterInput.WantsToStrafe)
	{
		TempCamMode = ECameraMode::Strafe;
	}
	if (CharacterInput.WantsToAim)
	{
		TempCamMode = ECameraMode::Aim;
	}
	RetCameraProperties.CameraMode = TempCamMode;

	return RetCameraProperties;
}

void APlayableCharacter::TargetLockAction(const FInputActionValue& Value)
{
	TargetLockSwitch = !TargetLockSwitch;
	TargetLockComponent->TargetLockSwitch(TargetLockSwitch);
}

void APlayableCharacter::InteractAction()
{
	if (IsValid(InteractComponent))
	{
		InteractComponent->Interact();
	}
}

void APlayableCharacter::UseItemAction()
{
	// QuickSlot Idx
	InventoryComponent_V2->UseItem(0, 1);
}

void APlayableCharacter::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}
	}
}

void APlayableCharacter::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputReleased(*Spec);
		}
	}
}

void APlayableCharacter::InputCombatAction(int32 InputID)
{
	if (!mPlayerCombatComp)
	{
		return;
	}

	mPlayerCombatComp->AddCombatInput(InputID);
 }

void APlayableCharacter::InputCombatCancleActivate()
{
	if (!mPlayerCombatComp)
	{
		return;
	}

	mPlayerCombatComp->ActivateCombatCancle(true);
}

void APlayableCharacter::InputCombatCancleUnActivate()
{
	if (!mPlayerCombatComp)
	{
		return;
	}

	mPlayerCombatComp->ActivateCombatCancle(false);
}

void APlayableCharacter::SwitchCamer(ECameraType CameraType)
{
	if (mCurrentCameraType == CameraType)
	{
		return;
	}
	mCurrentCameraType = CameraType;
	USpringArmComponent* TargetSpringArm = nullptr;
	switch (CameraType)
	{
	case ECameraType::Main:
		TargetSpringArm = DefaultCameraBoom;
		break;
	case ECameraType::FinisherFront:
		TargetSpringArm = FinisherFrontCameraBoom;
		break;
	case ECameraType::FinisherBack:
		TargetSpringArm = FinisherBackCameraBoom;
		break;
	}

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	MainCamera->AttachToComponent(TargetSpringArm, AttachmentTransformRules);
	
	if (ECameraType::Main != CameraType)
	{
	}
	else
	{

	}
}

void APlayableCharacter::TestApplyEffect(TSubclassOf<UGameplayEffect> TestGE)
{
	if (!AbilitySystemComponent || !TestGE)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	EffectContext.AddInstigator(this, this);
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(TestGE, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void APlayableCharacter::Test_PrintString(const FString& DebugMessage)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, DebugMessage);
}

void APlayableCharacter::Test_AddGameplayTag(const FName& AddTagName)
{
	FGameplayTag AddTag = FGameplayTag::RequestGameplayTag(AddTagName);

	FGameplayTagContainer TagConainer;
	TagConainer.AddTag(AddTag);

	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(this, TagConainer);
}

void APlayableCharacter::Test_RemoveGameplayTag(const FName& RemoveTagName)
{
	FGameplayTag RemoveTag = FGameplayTag::RequestGameplayTag(RemoveTagName);

	FGameplayTagContainer TagConainer;
	TagConainer.AddTag(RemoveTag);

	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, TagConainer);
}

void APlayableCharacter::Test_DirectDead()
{
	mOnDead.Broadcast();
}

void APlayableCharacter::StopTargetLock(bool Value)
{
	if (!IsValid(TargetLockComponent))
	{
		return;
	}

	TargetLockComponent->SetActiveTargetTrace(Value);
}

//FVector APlayableCharacter::GetLockPoint()
//{
//	return GetMesh()->GetSocketLocation(FName("neck_01"));
//}

AActor* APlayableCharacter::GetTarget() const
{
	return TargetLockComponent->GetTargetActor();
}



void APlayableCharacter::SetTimeDilation(float NewDilationTime)
{
	UE_LOG(LogTemp, Error, TEXT("Float Value : %f"), NewDilationTime);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), NewDilationTime);
}

UEquipComponent* APlayableCharacter::GetEquipComponent()
{
	return EquipComponent;
}

UQuestComponent* APlayableCharacter::GetQuestComponent()
{
	return QuestComponent;
}

UInventoryComponent_V2* APlayableCharacter::GetInventoryComponent()
{
	return InventoryComponent_V2;
}

void APlayableCharacter::BindDelegateToAbilitySystem()
{
	Super::BindDelegateToAbilitySystem();

	AbilitySystemComponent->RegisterGameplayTagEvent(State::State_WeaponActivated, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &APlayableCharacter::EventUpdateAnimByEquipWeapon);
	
	// 캐릭터 태그기반 애니메이션 트리거 
	AbilitySystemComponent->RegisterGameplayTagEvent(
		State::State_Blocking, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayableCharacter::EventUpdateAnimByBlockState);

	//TArray<FGameplayAttribute> Attributes;
	//AbilitySystemComponent->GetAllAttributes(Attributes);
	//for (auto& Attribute : Attributes)
	//{
	//	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
	//		.AddLambda([](const FOnAttributeChangeData& ChangedData) {
	//			
	//			UE_LOG(LogTemp, Display, TEXT("CallbackTest %s"), *ChangedData.Attribute.GetName());

	//		});
	//}
}

void APlayableCharacter::EventDeath(FGameplayTag Tag, int32 NewCount)
{
	Super::EventDeath(Tag, NewCount);
	//if (false == AbilitySystemComponent->HasMatchingGameplayTag(TAG_STATE_DEATH))
	//{
	//	FTimerHandle TimerHandle;

	//	// 플레이어 인풋막기 
	//	// IMC 를 교체해서, 클릭하면 다시 살아나게 함 
	//	// 이때는 어빌리티를 만들어서 관리하고, 이 어빌리티 내부에서 
	//	// 플레리어의 체력을 다시 재성해줘야함 

	//	//여기서 살아나게되었다면, 기존에 재생중이던  UI 애니메이션 중지가 필요하다 

	//	// 만약 다시 살아나지 않는다면 로딩 페이지로 넘어가야함 
	//	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	//	if (IsValid(EnhancedInputLocalPlayerSubsystem))
	//	{
	//		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Dead, 2);

	//		bool bHasIMC_Finisher = EnhancedInputLocalPlayerSubsystem->HasMappingContext(IMC_Dead);
	//		if (bHasIMC_Finisher)
	//		{
	//			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	//			if (IsValid(EnhancedInputComponent))
	//			{
	//				EnhancedInputComponent->BindAction(IA_Resurrection, ETriggerEvent::Started, this, &APlayableCharacter::Resurrection);
	//			}
	//		}
	//	}
	//}
}

void APlayableCharacter::EventUpdateAnimByEquipWeapon(const FGameplayTag Tag, int32 NewCount)
{
	if (0 == NewCount)
	{
		const FLinkAnimInstanceData* LAID = UGameDataSettings::Get()->GetDataTableRow<FLinkAnimInstanceData>(EGlobalGameDataTableType::LinkAnimData, "Idle");
		if (!LAID && !LAID->LinkAnimInstance)
		{
			return;
		}

		GetMesh()->LinkAnimClassLayers(LAID->LinkAnimInstance);
	}

	else if (NewCount == 1)
	{
		const FGameplayTag& EquipItemTag = EquipComponent->GetEquipWeaponTag();
		if (!EquipComponent->GetEquipWeaponTag().IsValid())
		{
			return;
		}

		const FLinkAnimInstanceData* LAID = UGameDataSettings::Get()->GetDataTableRow<FLinkAnimInstanceData>(EGlobalGameDataTableType::LinkAnimData, *EquipItemTag.ToString());
		if (!LAID || !LAID->LinkAnimInstance)
		{
			return;
		}

		MY_LOG(Error, "애니메이션 존재확인");

		GetMesh()->LinkAnimClassLayers(LAID->LinkAnimInstance);
	}
}

void APlayableCharacter::EventUpdateAnimByBlockState(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount >= 1)
	{
		SetStance(EStance::Block);
	}
	else
	{
		SetStance(EStance::Normal);
	}
}

void APlayableCharacter::SetFinisher(bool NewValue)
{
	if (NewValue)
	{
		bFinisherSetting = NewValue;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (IsValid(PlayerController))
		{
			PostProcessBlackAndWhileEffect();

			UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (IsValid(EnhancedInputLocalPlayerSubsystem))
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Finisher, 1);

				bool bHasIMC_Finisher = EnhancedInputLocalPlayerSubsystem->HasMappingContext(IMC_Finisher);
				if (bHasIMC_Finisher)
				{
					UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
					if (IsValid(EnhancedInputComponent))
					{
						//EnhancedInputComponent->BindAction(IA_Finisher, ETriggerEvent::Started, this, &APlayableCharacter::Finisher);
					}
				}
			}
		}

	}
	else
	{
		bFinisherSetting = NewValue;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (IsValid(PlayerController))
		{
			UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			bool bHasIMC_Finisher = EnhancedInputLocalPlayerSubsystem->HasMappingContext(IMC_Finisher);
			{
				if (IsValid(EnhancedInputLocalPlayerSubsystem))
				{
					EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(IMC_Finisher);
				}
			}
		}
	}
}

void APlayableCharacter::Slowmotion()
{
	SlowmotionTimelineComponent->PlayFromStart();
}

void APlayableCharacter::PostProcessBlackAndWhileEffect()
{
	//FadeValue = FMath::FInterpTo(FadeValue, 1.f, GetWorld()->GetDeltaSeconds(), 1.0f);
	//mPostProcessMtrls[0]->SetScalarParameterValue(TEXT("FadeRadius"), 1.f);
	//
	//if (FadeValue < 1.f)
	//{
	//	FTimerHandle Handle; 
	//	GetWorld()->GetTimerManager().SetTimer(Handle, [this]() { PostProcessBlackAndWhileEffect(); }, 0.01f, false);
	//}
}


#pragma once
#include "CoreMinimal.h"
#include "CharacterInfo.generated.h"

UENUM(BlueprintType)
enum class EDamagedDirection : uint8
{
	Left = 0,
	Right,
	Fwd,
	Back,
	None = 255,
};

UENUM(BlueprintType)
enum class EGait : uint8
{
	None	UMETA(DisplayName = "None"),
	Jog		UMETA(DisplayName = "Jog"),
	Run		UMETA(DisplayName = "Run"),
};

UENUM(BlueprintType)
enum class EStance : uint8
{
	None	UMETA(DisplayName = "None"),
	Normal	UMETA(DisplayName = "Normal"),
	Block	UMETA(DisplayName = "Block"), /*여기서 Block 제거하고 Block 은 Blocking 을 확인하는게 좋아보인다..*/
	Crouch	UMETA(DisplayName = "Crouch"),
};


UENUM(BlueprintType)
enum EFraction : uint8
{
	Friends = 0,
	Enemise = 1,
	Civilians = 255,
};

UENUM(BlueprintType)
enum EAbilitySlot : int
{
	LightAttack,
	HeavyAttack,
	SecondaryAttack,
	Block,
};


UENUM(BlueprintType)
enum EWeaponSlot : int
{
	RightHand,
	LeftHand,
	TwoHand,
};

UENUM(BlueprintType)
enum class ECameraStyle : uint8
{
	None UMETA(DisplayName = "None"),
	Far UMETA(DisplayName = "Far"),
	Balanced UMETA(DisplayName = "Balanced"),
	Close UMETA(DisplayName = "Close"),
};

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	None UMETA(DisplayName = "None"),
	FreeCam UMETA(DisplayName = "FreeCam"),
	Strafe UMETA(DisplayName = "Strafe"),
	Aim UMETA(DisplayName = "Aim"),
};

USTRUCT(BlueprintType)
struct GASSTUDY_API FPlayerPropertiesForCamera
{
	GENERATED_BODY()
	
	FPlayerPropertiesForCamera() {}

	FPlayerPropertiesForCamera(ECameraStyle InCamStyle, ECameraMode InCamMode, EStance InStance)
		: CameraStyle(InCamStyle)
		, CameraMode(InCamMode)
		, Stance(InStance)
	{
	}

	UPROPERTY(BlueprintReadWrite)
	ECameraStyle CameraStyle = ECameraStyle::None;

	UPROPERTY(BlueprintReadWrite)
	ECameraMode CameraMode = ECameraMode::None;

	UPROPERTY(BlueprintReadWrite)
	EStance	Stance = EStance::None;
};


USTRUCT(BlueprintType)
struct FCharacterInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool WantsToSprint = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool WantsToWalk = true;

	UPROPERTY(BlueprintReadWrite)
	bool WantsToStrafe = false;

	UPROPERTY(BlueprintReadWrite)
	bool WantsToAim = false;
};

USTRUCT(BlueprintType)
struct FCameraInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SpringArmLength = 0.f;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector::Zero();

	UPROPERTY(EditAnywhere)
	float TranslationLagSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float FieldOfView = 0.f;

	UPROPERTY(EditAnywhere)
	float TransitionSpeed = 0.f;
};

UENUM(BlueprintType)
enum class ECounterType : uint8
{
	None,
	DodgeDodge UMETA(DisplayName = "Dodge"),
	Block UMETA(DisplayName = "Block"),
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"


/**
* 타겟이 지정되었을때, 해당 타겟의 ASC 기반으로 보스몬스터의 위젯을 UI, 
* 혹은, 자기 자신의 머리위에 UI 를 보여주는데, 이때 View 모델 생성이 필요하다. 
* 
* ViewModel 은 동적으로 생성된다.
* 
* 쫄몹들은 그냥 자기가 뷰모델을 가지고 있다가 근천에서 싸우고있으면 
* 위젯이 보이면 그면임
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargeted, AActor*, TargetedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetReleased);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	void SetOwnerRotator();

	//void EndTargerLock();
	bool TargetIsAlive(AActor* Target);
	FRotator GetLockOnCameraRotation();


	// 새로운 타겟 락 시스템 
	/*
	일정 시간마다 캐릭터 주변의 적을 스피어 콜리전을 통해 탐색한다.

	하나의 몬스터가 탐색되면,
	현재 포커싱한 타겟이 된다.

	다수의 몬스터가 탐색된다면, 
	가장 우선순위가 높은 타겟 하나와, 그외의 타겟들을 저장한다.


	메인 타겟이 고정된다면, 적 탐색을 멈춘다.
	그러나, 메인 타겟과 일정 거리이상 멀어지거나 타겟이 죽으면

	다시 탐색을 시작한다.


	*/


	// PlayerInput 
public:
	void TargetLockSwitch(bool Switch);

	// Lock On/Off
	void TargetLockOn();
	void TargetLockOff();

	// Handler Function 
	void CheckTargetHandler_V2(bool Switch);
	void LockOnHandler(bool Switch);

	// Check Function
	void CheckTargetIsValid_V2();
	bool CheckTargetIsInPlayerSight(AActor* CheckTarget, float PlayerSightRange, bool UseOwnerFwdVector = true, FVector NewFwdVector = FVector::ZeroVector);
	bool CheckObsatacleBetweenOwnerAndTarget(AActor* CheckTarget);


	// Main Logic
	UFUNCTION()
	void FindTarget();
	void LockOn();


	void SetOwnerStarfe(bool Switch);


public:
	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;

	//UFUNCTION(BlueprintCallable)
	//void TargetLockOnOffSwitch();

	UFUNCTION(BlueprintCallable)
	void SwitchTargetLeft();
	
	UFUNCTION(BlueprintCallable)
	void SwitchTargetRight();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTargetLocking() { return TargetActor.IsValid(); }

	UFUNCTION(BlueprintCallable)
	void SetActiveTargetTrace(bool Value) { ActiveTargetTrace = Value; }

	UFUNCTION(BlueprintCallable)
	bool GetActiveTargetTrace() { return ActiveTargetTrace;}

	AActor* TraceTargetByForward(FVector FwdDir);

	void TargetLockOff_IsObstacle();



private:
	// Rotation
	void SetControllRotationOnTarget();
	FRotator GetControlRotationOnTarget() const;
	float GetDistanceFromOwner(const AActor* OtherActor) const;


private:
	enum class Direction : uint8
	{
		Left = 1,
		Right,
	};

	void SearchLeftRight(Direction InDirection);

	TArray<AActor*> TraceSphereRangeTargets(bool CheckPlayerSight = false);


	UPROPERTY(EditAnywhere, category = TraceRadius)
	float LockRadius = 1000.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPirvateAccess = true))
	float SwitchSpeed = 0.01f;
	UPROPERTY(EditAnywhere, meta = (AllowPirvateAccess = true))
	float NormalSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	TObjectPtr<class UCameraComponent> OwnerCamera;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<ACharacter> TargetActor;

	UPROPERTY()
	FTimerHandle TargertLockTimerHandle;

	UPROPERTY(VisibleAnywhere)
	bool ActiveTargetTrace = true;

	//UPROPERTY(BlueprintAssignable)
	//FOnTargetLocked OnLock;
	//UPROPERTY(BlueprintAssignable)
	//FEndLock EndLock;


	// 트레이스 타이머 
	FTimerHandle CollisionTestTimerHandle;

	UPROPERTY(EditAnywhere)
	int32 FrameTime = 60.f;

	// 60 프레임 기준, 한프레임 한번 실행할 수 있는 시간으로 설정 
	UPROPERTY(VisibleAnywhere)
	float CollisionTestRate = 1.0f / FrameTime;


	FTimerHandle CheckTargetTimerHandle;


	const float PlayerSight = 140.f;

	// 하드락 로테이어 타미어 
	FTimerHandle HardLockRotateTimerHandle;


	UPROPERTY(EditInstanceOnly)
	float TargetRemoveDelay = 2.0f;

	FTimerHandle TargetRemoveTimerHnadle;


	bool bHasObstacleBetweenOwnerAndTarget = false;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwnerPlayerController;


public:
	// Then Clamped by PitchMin / PitchMax
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	bool bAdjustPitchBasedOnDistanceToTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchDistanceCoefficient = -0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchDistanceOffset = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchMin = -50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchMax = -20.0f;

	// Whether to accept pitch input when bAdjustPitchBasedOnDistanceToTarget is disabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	bool bIgnoreLookInput = true;

	UPROPERTY()
	TWeakObjectPtr<APawn> OwnerPawn;


	FTimerHandle LockOnTimerHandle;

	FTimerHandle TargetRemoveByObstacleTimerHandle;

	FTimerHandle TargetChangedTargetHandle;

	UPROPERTY(EditAnywhere)
	float TargetRemoveDelayByObstacle = 2.0f;

	float InterpSpeed = 9.0f;

	bool TargetIsDodge = false;

	FName mTargetSocketName = TEXT("TargetSocket");
};


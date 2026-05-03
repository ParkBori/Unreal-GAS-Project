// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/Commbat/CombatData.h"
#include "Shared/Weapon/WeaponData.h"
#include "WeaponTraceComponent.generated.h"


/**
* 충돌 정보를 외부에서 사용하기 위한 델리게이트
*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDetecedHit, const TArray<FHitResult>&);


namespace EIntervalTraceCountByTraceShape
{
	enum Type : uint8
	{
		LINE = 3,
		CAPSULE = 5,
	};
}



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UWeaponTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponTraceComponent();

public:
	bool Init(AActor* Actor, FName SocketStart, FName SocketEnd, EWeaponTraceShape WeaponTraceShape);
	bool SetTraceData(float NotifyStartTime, FName CurrentAttackName);
	void Reset();
	
	bool GetActivate() { return bActivate; }

	void Trace();

private:
	bool SetAnimData();
	void AccumulateParent2RootBoneTransform(FTransform& AccumulatedLocalTransform, const FFrameTime& FrameTime);
	void LineTrace(FVector Start, FVector End, bool Debug);
	void CapsuleTrace(FVector Start, FVector End, bool Debug);

	void MyDrawLine(FVector Start, FVector End, FColor Color, float Time);
	void MyDrawSphere(FVector Center, FColor Color, float Time);
	void MyDrawCapsule(FVector Start, FVector End, float HalfHeight, float Radius, FQuat Rotation, FColor Color, float Time);
	void ConnectLineToLine();
	void GetWorldSocketTransform(FTransform& Start, FTransform& End, const FFrameTime& FrameTime, const FTransform& ToWorld);

	void SetWeaponTraceShape(EWeaponTraceShape NewShape);
	void SetInterpCount(int32 NewCount);

	void CollisionTestByTraceShape(FVector Start, FVector End, bool Debug);

	void SetActivate(bool Value) { bActivate = Value; }



private:

	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	FName	mWeaponAttachedSocketStart;

	UPROPERTY()
	FName	mWeaponAttachedSocketEnd;

	
	UPROPERTY()
	int32 mAnimIndx = 0;

	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> mMeshComp;

	UPROPERTY()
	TObjectPtr<class UAnimInstance> mAnimInst;

	UPROPERTY()
	TScriptInterface<IAnimationDataModel> mDataModelInterface;

	UPROPERTY()
	TObjectPtr<UAnimMontage> mCurrentPlayingMontage;

	UPROPERTY(VisibleAnywhere, Category = "Trace", meta = (DisplayName = "WeaponTraceShape"))
	EWeaponTraceShape mWeaponTraceShape = EWeaponTraceShape::Line;

	float mNotifyStartTime = 0.f;

	float mCurrentMontagePos = 0.f; // 현재 몽타주 재생 위치 
	float mPrevMontagePos = 0.f; // 이전 몽타주 재생 위치 

	double mOneFPSInterval = 0.0; // 1 프레임에 걸리는 시간 ex) 120 fps => 1 / 120 
	int32 mInterpCount = 3; // 1 프레임 사이에 보간할 횟수 
	double mlerpInterval = 0.0; // 1 프레임에 3 번 보간한다면, 몇초에 한번씩 일어나야하는지 ex) 1 / 120 / mInterpCount 

	FFrameRate mCurrentMontageFrameRate;
	FTransform mPrevMeshTransform;
	FTransform mCurrentMeshTransform;

	// 
	FVector mCurrentStartLocation = FVector::Zero();
	FVector mCurrentEndLocation = FVector::Zero();
	FVector mPrevStartLocation = FVector::Zero();
	FVector mPrevEndLocation = FVector::Zero();

	// 충돌처리 
	// 충돌체 내부에서 레퍼런스를 사용할수없음
	TMap<const FString, const FHitResult> mHitResults;

	UPROPERTY();
	TSet<FName> mHitDetectedActor;

	FName AttackName; 
	
	UPROPERTY(VisibleAnywhere)
	bool bActivate = false;

	UPROPERTY(EditAnywhere)
	bool CanDrawDebug = false; 

	bool HitStop = false;


	FTransform LastStartTransform;
	FTransform LastEndTransform;

public:
	FOnDetecedHit mOnDetecedHit;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FootIKComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UFootIKComponent::UFootIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFootIKComponent::BeginPlay()
{
	Super::BeginPlay();
	
	mOwnerCharacter = Cast<ACharacter>(GetOwner());

	if (mOwnerCharacter)
		mCapsuleHalfHeight = mOwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void UFootIKComponent::TickComponent(float DeltaTime,
	enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UFootIKComponent::UpdateFootIK(float DeltaTime)
{
	if (!IsValid(mOwnerCharacter))
		return;

	FSkeletonIKTraceInfo	LeftTrace = FootTrace(TraceDistanceOffset, FName("foot_l"), mLeftFootGoalPosition);
	FSkeletonIKTraceInfo	RightTrace = FootTrace(TraceDistanceOffset, FName("foot_r"), mRightFootGoalPosition);

	UpdateFootRotation(DeltaTime, NormalToRotator(LeftTrace.ImpactNormal), mLeftFootRotation, 16.f);
	UpdateFootRotation(DeltaTime, NormalToRotator(RightTrace.ImpactNormal), mRightFootRotation, 16.f);


	mLeftFootOffset = LeftTrace.Offset;
	mRightFootOffset = RightTrace.Offset;

	//UE_LOG(LogTemp, Warning, TEXT("%f ---- %f"), mLeftFootOffset, mRightFootOffset);


	mHipsOffset = FMath::Min(mLeftFootOffset, mRightFootOffset);
}

FSkeletonIKTraceInfo UFootIKComponent::FootTrace(float TraceDistance, const FName& SocketName, FVector& GoalPosition)
{
	FSkeletonIKTraceInfo	TraceInfo;

	TraceInfo.ImpactNormal = FVector::UpVector;

	if (!IsValid(mOwnerCharacter))
		return TraceInfo;

	// 현재 foot location 
	FVector	SocketLocation = mOwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	FVector	RootLocation = mOwnerCharacter->GetMesh()->GetSocketLocation(TEXT("root"));
	// 현재 발 소켓의 X,Y, 캐릭터의 z 좌표 (왜 캐릭터 z 좌표인가, 발이 있을 수 있는 최대치 같은느낌)
	FVector	StartPoint = FVector(SocketLocation.X, SocketLocation.Y, RootLocation.Z + 30);
	// 현재 박에서 
	//FVector	EndPoint = StartPoint - FVector::UpVector * (mCapsuleHalfHeight + TraceDistance);
	FVector	EndPoint = FVector(SocketLocation.X, SocketLocation.Y, RootLocation.Z - 50);

	FHitResult	Hit;
	TArray<AActor*>	IgnoreActors;

	IgnoreActors.Add(mOwnerCharacter);

	bool Collision = UKismetSystemLibrary::LineTraceSingle(GetWorld(), 
		StartPoint,	EndPoint, 
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, IgnoreActors, EDrawDebugTrace::ForOneFrame, Hit, true);

	if (Collision)
	{
		// 충돌 지점의 수직인 벡터 
		TraceInfo.ImpactNormal = Hit.Normal;
		//TraceInfo.Offset = (Hit.ImpactPoint - EndPoint).Size() - TraceDistance + 3.f;
		TraceInfo.Offset = (Hit.ImpactPoint.Z - RootLocation.Z);

		TraceInfo.ImpactPoint = Hit.ImpactPoint;
	}

	return TraceInfo;
}

FRotator UFootIKComponent::NormalToRotator(const FVector& Normal)
{
	double Roll = UKismetMathLibrary::DegAtan2(Normal.Y, Normal.Z);
	double Pitch = -UKismetMathLibrary::DegAtan2(Normal.X, Normal.Z);

	return FRotator(Pitch, 0.0, Roll);
}

void UFootIKComponent::UpdateFootRotation(float DeltaTime, const FRotator& TraceRotation, FRotator& CurrentRotation, float InterpSpeed)
{
	CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TraceRotation, DeltaTime, InterpSpeed);
}


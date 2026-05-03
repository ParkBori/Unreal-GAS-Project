// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootIKComponent.generated.h"

USTRUCT(BlueprintType)
struct FSkeletonIKTraceInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float	Offset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector	ImpactNormal = FVector::Zero();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector	ImpactPoint = FVector::Zero();
};



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GASSTUDY_API UFootIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFootIKComponent();

public:
	FRotator GetLeftRotation()	const
	{
		return mLeftFootRotation;
	}

	FRotator GetRightRotation()	const
	{
		return mRightFootRotation;
	}

	float GetLeftOffset() const
	{
		return mLeftFootOffset;
	}

	float GetRightOffset()	const
	{
		return mRightFootOffset;
	}

	float GetHipOffset()	const
	{
		return mHipsOffset;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateFootIK(float DeltaTime);

private:
	FSkeletonIKTraceInfo FootTrace(float TraceDistance, const FName& SocketName, FVector& GoalPosition);
	FRotator NormalToRotator(const FVector& Normal);
	void UpdateFootRotation(float DeltaTime, const FRotator& TraceRotation,
		FRotator& CurrentRotation, float InterpSpeed);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	float	mLeftFootOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	float	mRightFootOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	FRotator	mLeftFootRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	FRotator	mRightFootRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	float		mHipsOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	FVector	mLeftFootGoalPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	FVector	mRightFootGoalPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FootIK")
	FVector		mHipsGoalPosition;

	class ACharacter* mOwnerCharacter = nullptr;

	float		mCapsuleHalfHeight = 0.f;

	// 캡슐 하단 끝부분에서 추가적으로 더 충돌판전을 할 거리
	UPROPERTY(EditAnywhere, Category = "FootIK")
	float	TraceDistanceOffset = 50.f;
};

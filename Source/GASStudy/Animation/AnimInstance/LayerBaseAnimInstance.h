// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LayerBaseAnimInstance.generated.h"


USTRUCT(BlueprintType)
struct FLocomotionAnimation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle|Normal")
    UAnimSequence* mIdle = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle|Block")
    UAnimSequence* mRightBlockIdle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
    UBlendSpace* mStart = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Idle")
    UBlendSpace* mIdleMove = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Block")
    UBlendSpace* mBlockMove = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stop")
    UBlendSpace* Stop = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump|Start")
    UAnimSequence* mJumpStart = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump|Loop")
    UAnimSequence* mJumpLoop = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump|End")
    UAnimSequence* mJumpEnd = nullptr;
};

/**
 * 
 */
UCLASS()
class GASSTUDY_API ULayerBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

protected:
    UPROPERTY(EditAnyWhere, BlueprintReadOnly)
    FLocomotionAnimation mLocomotionAnim;
    
    UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UGASAnimInstance> mBaseAnim;

};
